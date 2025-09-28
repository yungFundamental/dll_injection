#include <windows.h>
#include <fstream>

using namespace std;


void hijackMessages() {
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    std::ofstream("C:\\dawger.txt") << "hello world";
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        hijackMessages();

    return TRUE;
}
