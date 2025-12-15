#include <windows.h>


WNDPROC prevWindowProc = nullptr;

HWND FindAncestorWindowByClass(HWND hParent, const char *className) {
    HWND hChild = FindWindowExA(hParent, nullptr, className, nullptr);
    if (hChild != nullptr) {
        return hChild;
    }

    hChild = FindWindowExA(hParent, nullptr, nullptr, nullptr); // Start with the first child
    while (hChild != nullptr) {
        HWND hGrandchild = FindAncestorWindowByClass(hChild, className);
        if (hGrandchild != nullptr) {
            return hGrandchild;
        }

        hChild = GetWindow(hChild, GW_HWNDNEXT);
    }

    return nullptr;
}

LRESULT CALLBACK myWindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    if (uMsg == WM_CHAR && wParam == 'c') {
        return CallWindowProcA(prevWindowProc, hwnd, uMsg, ' ', lParam);
    }

    return CallWindowProcA(prevWindowProc, hwnd, uMsg, wParam, lParam);
}

void replaceWindowProc() {
    char debug_msg[128];
    OutputDebugStringA("Hook: Entered custom replaceWindowProc\n");
    HWND npHwnd = FindWindowEx(nullptr, nullptr, "Notepad", nullptr);
    HWND hEditWnd = FindAncestorWindowByClass(npHwnd, "RichEditD2DPT");
    wsprintfA(debug_msg, "Hook: EditWindow handle allocated to 0x%p\n", hEditWnd);
    OutputDebugStringA(debug_msg);

    prevWindowProc = reinterpret_cast<WNDPROC>(GetWindowLongPtrA(hEditWnd, GWLP_WNDPROC));
    SetWindowLongPtrA(hEditWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(myWindowProc));
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        replaceWindowProc();
    return TRUE;
}
