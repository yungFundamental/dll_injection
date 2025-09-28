#include <iostream>
#include <Windows.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <window_class>" << std::endl;
        std::cout <<
                "Note, this program will only find processes with standard window classes like Notepad, Button, etc" <<
                std::endl;
        return 0;
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

    std::cout << "Closing..." << std::endl;
    CloseHandle(proc);
    return 0;
}
