#include <iostream>
#include <Windows.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <process_name>" << std::endl;
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

    return 0;
}
