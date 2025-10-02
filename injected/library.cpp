#include <windows.h>

LONG_PTR prevWindowProc = 0;

LRESULT CALLBACK myWindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    if (uMsg == WM_CHAR) {
        hwnd = CreateWindowA("STATIC", "Hello", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 500, 100, nullptr,
                                  nullptr, nullptr, nullptr);
    }

    return CallWindowProcA(reinterpret_cast<WNDPROC>(prevWindowProc), hwnd, uMsg, ' ', lParam);
}

void replaceWindowProc() {
    HWND npHwnd = FindWindowEx(NULL, NULL, "Notepad", NULL);
    HWND hEditWnd = GetWindow(npHwnd, GW_CHILD);
    CreateWindowA("STATIC", "Hello", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 500, 100, NULL, NULL, NULL, NULL);

    prevWindowProc = GetWindowLongPtrA(hEditWnd, GWLP_WNDPROC);
    SetWindowLongPtrA(hEditWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(myWindowProc));
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    CreateWindowA("STATIC", "Hello", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 500, 100, NULL, NULL, NULL, NULL);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        replaceWindowProc();
    return TRUE;
}
