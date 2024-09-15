#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_impl_win32.h"
#include "minhook/include/MinHook.h"
#include "aimbot.hpp"
#include "esp.hpp"
#include "movement.hpp"
#include "misc.hpp"
#include "navigation.hpp"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef BOOL(WINAPI* tSwapBuffers)(HDC hdc);
tSwapBuffers oSwapBuffers = nullptr;

WNDPROC oWndProc = nullptr;

LRESULT CALLBACK newWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI hkSwapBuffers(HDC hdc);
void InitializeHook();


namespace hack
{
    void cycle() {
        AimBot::cycle();
        ESP::cycle();
        Movement::cycle();
        Misc::cycle();
    }

    void init() {
        ImGui::CreateContext();
        HWND hWnd = FindWindowA(NULL, "AssaultCube");
        oWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)newWndProc);
        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplOpenGL2_Init();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetCurrentContext(ImGui::GetCurrentContext());
        InitializeHook();
        Nav::init();
    }


    void render() {
        ImGui::SetCurrentContext(ImGui::GetCurrentContext());
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        hack::cycle();

        //              NAVIGATION + DRAW
        if (settings::menu::bShow)
        {
            Nav::drawMenu();
            Nav::processInput();
        }

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }
}

LRESULT CALLBACK newWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (settings::menu::bShow) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return true;
    }
    else
    {
        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }
}


BOOL WINAPI hkSwapBuffers(HDC hdc) {

    hack::render();

    return oSwapBuffers(hdc);
}

void InitializeHook() {
    if (MH_Initialize() != MH_OK) {
        MessageBox(NULL, "MinHook initialization failed", "Error", MB_ICONERROR);
        return;
    }
    void* pSwapBuffers = (void*)GetProcAddress(GetModuleHandleA("gdi32.dll"), "SwapBuffers");
    if (MH_CreateHook(pSwapBuffers, &hkSwapBuffers, reinterpret_cast<void**>(&oSwapBuffers)) != MH_OK) {
        MessageBox(NULL, "Failed to create hook for SwapBuffers", "Error", MB_ICONERROR);
        return;
    }
    if (MH_EnableHook(pSwapBuffers) != MH_OK) {
        MessageBox(NULL, "Failed to enable hook for SwapBuffers", "Error", MB_ICONERROR);
        return;
    }
}