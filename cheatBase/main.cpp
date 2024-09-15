#include <windows.h>
#include <iostream>
#include <cmath>
#include "hack.hpp"
#pragma comment(lib, "User32.lib")

POINT lastCursorPos;

void AllocateConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
}

void ProcessKeyInput() {
    if (GetAsyncKeyState(VK_NUMPAD5) & 1) { // Use & 1 to detect key press once
        settings::menu::bShow = !settings::menu::bShow;
    }
}

void PrintPlayerInfo() {
    while (true) {
        Sleep(15);
        mem.init();
        ProcessKeyInput();
    }
}

DWORD WINAPI MainThread(LPVOID param) {
    mem.init();
    hack::init();
    AllocateConsole();
    PrintPlayerInfo();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
    }
    return TRUE;
}
