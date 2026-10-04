#include "hooks.hpp"
#include <sdk/offsets.hpp>

#include <stdexcept>
#include <MinHook.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <features/esp.hpp>

void Hooks::Setup() {

    if (MH_Initialize())
        throw std::runtime_error("Unable to init minhook");

    if (MH_CreateHook(VirtualFunction(GUI::device, 42), &EndScene, reinterpret_cast<void**>(&oEndScene)))
        throw std::runtime_error("Unable to hook EndScene");
    
    if (MH_CreateHook(VirtualFunction(GUI::device, 16), &Reset, reinterpret_cast<void**>(&oReset)))
        throw std::runtime_error("Unable to hook Reset");

    if (MH_CreateHook(reinterpret_cast<LPVOID>(offsets::fn::CL_ParseSnapshot), &hk_ParseSnapshot, reinterpret_cast<LPVOID*>(&oParseSnapshot)))
        throw std::runtime_error("Unable to hook CL_ParseSnapshot");

    if (MH_CreateHook(reinterpret_cast<LPVOID>(mem.modBase + offsets::fn::ScreenshotRequest), &hk_ScreenshotRequest, reinterpret_cast<LPVOID*>(&oScreenshotRequest)))
        throw std::runtime_error("Unable to hook ScreenshotRequest");

    if (MH_EnableHook(MH_ALL_HOOKS))
        throw std::runtime_error("Unable to enable hooks");

  GUI::DestroyDirectX();
}

void Hooks::Destroy() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
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
  
  if (GUI::open)
    GUI::RenderMenu();

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

void __cdecl Hooks::hk_ParseSnapshot(void* msg) {
    oParseSnapshot(msg);
    if (globals && globals->IsInGame) {
        EntityManager em;
        em.UpdateEntities();
    }
}

// needs testing
int __cdecl Hooks::hk_ScreenshotRequest(int msg, int command) {
    globals->FrameCounter = 3;

    return oScreenshotRequest(msg, command);
}