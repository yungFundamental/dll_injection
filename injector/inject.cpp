#include <iostream>
#include <Windows.h>
#include <filesystem>


int main(int argc, char *argv[]) {
    std::cout.setf(std::ios::unitbuf);  // Remove stdout buffering
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <window_class> <dll_path>" << std::endl;
        std::cout <<
                "Note, this program will only find processes with standard window classes like Notepad, Button, etc" <<
                std::endl;
        return 0;
    }

    if (!std::filesystem::exists(argv[2])) {
        std::cerr << "File \"" << argv[2] << "\" does not exist" << std::endl;
        return 1;
    }

    std::cout << "Starting inject process on \"" << argv[1] << "\" process..." << std::endl;

    const LPSTR windowName = argv[1];
    HWND windowHandle = FindWindowEx(nullptr, nullptr, windowName, nullptr);

    if (windowHandle == nullptr) {
        std::cerr << "Failed to find any window." << std::endl;
        return 1;
    }

    DWORD processId = 0;
    if (GetWindowThreadProcessId(windowHandle, &processId) == 0) {
        std::cerr << "Failed to get process id." << std::endl;
        return 1;
    }

    std::cout << "Found process with PID of " << processId << std::endl;

    std::cout << "Openning process..." << std::endl;
    HANDLE proc = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, processId);
    if (proc == nullptr) {
        std::cerr << "Failed OpenProcess" << std::endl;
        return 1;
    }

    std::cout << "Allocating memory..." << std::endl;
    void *remote_addr = VirtualAllocEx(proc, nullptr, 1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (remote_addr == NULL) {
        std::cerr << "Failed VirtualAllocEx" << std::endl;
        return 1;
    }

    std::string dll_path = argv[2];
    SIZE_T written = 0;

    std::cout << "Writing dll path of " << dll_path.data() << " (" << dll_path.size() + 1 << " bytes)..." << std::endl;
    if (WriteProcessMemory(proc, remote_addr, dll_path.data(), dll_path.size() + 1, &written) == 0) {
        std::cout << "Failed WriteProcessMemory" << std::endl;
        return -1;
    }
    std::cout << "Done writing " << written << " bytes" << std::endl;

    DWORD tid = 0;
    std::cout << "Creating thread..." << std::endl;
    const HANDLE th = CreateRemoteThread(proc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA),
                                         remote_addr, 0, &tid);
    if (th == nullptr) {
        std::cout << "Failed CreateRemoteThread" << std::endl;
        return -1;
    }
    std::cout << "Waiting for thread..." << std::endl;
    WaitForSingleObject(th, INFINITE);
    DWORD exit_code;
    GetExitCodeThread(th, &exit_code);
    if (exit_code == 0) {
        std::cerr << "Failed running DLL..." << std::endl;
        return 1;
    }
    std::cout << "DLL loaded, module base: 0x" << std::hex << exit_code << std::dec << "\n";
    std::cout << "Closing..." << std::endl;
    CloseHandle(proc);
    return 0;
}
