#include "hooks.hpp"
#include "features/aimbot.hpp"
#include <sdk/offsets.hpp>

#include <stdexcept>
#include <MinHook.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <features/esp.hpp>
#include <render/menu.hpp>
#include <common.hpp>

void Hooks::Setup() {

    if (MH_Initialize())
        throw std::runtime_error("Unable to init minhook");

    if (MH_CreateHook(VirtualFunction(GUI::device, 42), &EndScene, reinterpret_cast<void**>(&oEndScene)))
        throw std::runtime_error("Unable to hook EndScene");

    if (MH_CreateHook(VirtualFunction(GUI::device, 16), &Reset, reinterpret_cast<void**>(&oReset)))
        throw std::runtime_error("Unable to hook Reset");

    if (MH_CreateHook(reinterpret_cast<LPVOID>(offsets::engine::CL_ParseSnapshot), &hk_ParseSnapshot, reinterpret_cast<LPVOID*>(&oParseSnapshot)))
        throw std::runtime_error("Unable to hook CL_ParseSnapshot");

    if (MH_CreateHook(reinterpret_cast<LPVOID>(Mem.Cod4xBase + offsets::engine::ScreenshotRequest), &hk_ScreenshotRequest, reinterpret_cast<LPVOID*>(&oScreenshotRequest)))
        throw std::runtime_error("Unable to hook ScreenshotRequest");

    if (MH_CreateHook(reinterpret_cast<LPVOID>(offsets::engine::Com_Error), &hk_ComError, reinterpret_cast<LPVOID*>(&oCom_Error)))
        throw std::runtime_error("Unable to hook Com_Error");

    if (MH_EnableHook(MH_ALL_HOOKS))
        throw std::runtime_error("Unable to enable hooks");

    initialized = true;

    GUI::DestroyDirectX();
}

void Hooks::Destroy() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    initialized = false; // dont think I need this but why not
}

long __stdcall Hooks::EndScene(IDirect3DDevice9 *device) {
    const auto result = oEndScene(device, device);

    if (!GUI::initialized)
      GUI::SetupMenu(device);

    // hide imgui rendering when SnapshotRequest is called
    if (globals->FrameCounter > 0) {
        globals->FrameCounter--;
        return result;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ESP::DrawPlayers();
    if (Settings::Aimbot::FOVCircle) {
        Aimbot::DrawFOVCircle();
    }

    ImGui::GetIO().MouseDrawCursor = GUI::open; // fix cursor not showing when the menu is open

    if (GUI::open) {
        Menu::Render(); // new menu
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return result;
}

HRESULT __stdcall Hooks::Reset(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *p) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    const auto result = oReset(device, device, p);
    ImGui_ImplDX9_CreateDeviceObjects();
    return result;
}

// game/engine functions
void __cdecl Hooks::hk_ParseSnapshot(void* msg) {
    oParseSnapshot(msg);
    if (globals && globals->IsInGame) {
        EntityManager em;
        em.UpdateEntities();
    }
}

// needs testing
int __cdecl Hooks::hk_ScreenshotRequest(int msg, int command) {
    info("ScreenshotRequest ran");
    globals->FrameCounter = 3;

    return oScreenshotRequest(msg, command);
}

// prevent crashing
void __cdecl Hooks::hk_ComError(int a1, char* Format, ...) {
    error("Com_Error ran");

    char buffer[0x1000u];
    va_list va; // [esp+20h] [ebp+10h] BYREF
    va_start(va, Format);
    //_vsnprintf(&Buffer, 0x1000u, Format, va);
    vsnprintf(buffer, sizeof(buffer), Format, va);

    oCom_Error(4, const_cast<char*>("%s"), buffer); // case 4 does nothing
}
