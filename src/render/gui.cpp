#include "gui.hpp"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <stdexcept>

#include <globals.hpp>
#include <settings.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

bool GUI::SetupWC(const char *wcName) {
  // create dummy wc
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = DefWindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetModuleHandle(nullptr);
  wc.hCursor = nullptr;
  wc.hIcon = nullptr;
  wc.hbrBackground = nullptr;
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = wcName;
  wc.hIconSm = nullptr;

  if (!RegisterClassEx(&wc))
    return false;

  return true;
}

void GUI::DestroyWC() { UnregisterClass(wc.lpszClassName, wc.hInstance); }

bool GUI::SetupWindow(const char *wndName) {
  hwnd = CreateWindow(wc.lpszClassName, wndName, WS_OVERLAPPEDWINDOW, 0, 0, 50,
                      50, nullptr, nullptr, wc.hInstance, nullptr);
  if (!hwnd)
    return false;

  return true;
}

void GUI::DestroyWindow() {
  if (hwnd)
    DestroyWindow(hwnd);
}

bool GUI::SetupDirectX() {
  const auto handle = GetModuleHandle("d3d9.dll");
  if (!handle)
    return false;

  using CreateFn = LPDIRECT3D9(__stdcall *)(UINT);

  const auto create =
      reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

  if (!create)
    return false;

  d3d9 = create(D3D_SDK_VERSION);
  if (!d3d9)
    return false;

  D3DPRESENT_PARAMETERS p = {};
  p.BackBufferWidth = 0;
  p.BackBufferHeight = 0;
  p.BackBufferFormat = D3DFMT_UNKNOWN;
  p.BackBufferCount = 0;
  p.MultiSampleType = D3DMULTISAMPLE_NONE;
  p.MultiSampleQuality = 0;
  p.SwapEffect = D3DSWAPEFFECT_DISCARD;
  p.hDeviceWindow = hwnd;
  p.Windowed = 1;
  p.EnableAutoDepthStencil = 0;
  p.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
  p.Flags = 0;
  p.FullScreen_RefreshRateInHz = 0;
  p.PresentationInterval = 0;

  if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, hwnd,
                         D3DCREATE_SOFTWARE_VERTEXPROCESSING |
                             D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
                         &p, &device) < 0)
    return false;

  return true;
}

void GUI::DestroyDirectX() {
  if (device) {
    device->Release();
    device = nullptr;
  }

  if (d3d9) {
    d3d9->Release();
    d3d9 = nullptr;
  }
}

void GUI::Setup() {
  if (!SetupWC("dummy"))
    throw std::runtime_error("Failed to create window class");

  if (!SetupWindow("dummyWnd"))
    throw std::runtime_error("Failed to create window");

  if (!SetupDirectX())
    throw std::runtime_error("Failed to create device");

  DestroyWindow();
  DestroyWC();

  //MessageBoxA(nullptr, "GUI has been setup", "dbg", MB_OK);
}

void GUI::SetupMenu(LPDIRECT3DDEVICE9 device) {
  auto p = D3DDEVICE_CREATION_PARAMETERS{};
  device->GetCreationParameters(&p);

  hwnd = p.hFocusWindow;

  oWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
      hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc)));

  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX9_Init(device);

  initialized = true;
}

void GUI::Destroy() {
  ImGui_ImplDX9_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWindowProc));

  DestroyDirectX();
}

void GUI::RenderMenu() {
    ImGui::Begin("Interim", &open);
    if (ImGui::Button("Unload")) { globals->Running = false; }

    ImGui::Checkbox("ESP", &Settings::ESP::Enabled);
    ImGui::Checkbox("Box", &Settings::ESP::Box);
    ImGui::Checkbox("Roccat ESP", &Settings::ESP::Roccat);

    ImGui::Checkbox("Ignore Dead", &Settings::ESP::IgnoreDead);
    ImGui::Checkbox("Ignore Team", &Settings::ESP::IgnoreTeam);

    if (ImGui::Checkbox("Third Person", &Settings::CVAR::ThirdPerson)) {
        if (Settings::CVAR::ThirdPerson)
            mem.WriteDvar(cvars::cg_thirdperson, 1);
        else
            mem.WriteDvar(cvars::cg_thirdperson, 0);
    }

    if (ImGui::Checkbox("No Smoke", &Settings::CVAR::NoSmoke)) {
        if (Settings::CVAR::NoSmoke)
            mem.WriteDvar(cvars::fx_draw, 0);
        else
            mem.WriteDvar(cvars::fx_draw, 1);
    }

    if (ImGui::Checkbox("No Jump Cooldown", &Settings::CVAR::NoJumpCooldown)) {
        if (Settings::CVAR::NoJumpCooldown)
            mem.WriteDvar(cvars::jump_slowdown, 0);
        else
            mem.WriteDvar(cvars::jump_slowdown, 1);
    }

    ImGui::Text("iw3mp base: 0x%X", mem.modBase);
    ImGui::Text("cod4x base: 0x%X", mem.cod4xBase);

    //ImGui::SliderFloat("Width", &Settings::Debug::width, 10.0f, 80.0f, "%.3f", 0);
    //ImGui::SliderFloat("Height", &Settings::Debug::height, 50.0f, 125.0f, "%.3f", 0);

    ImGui::End();
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wideParam, LPARAM longParam) {
  if (GetAsyncKeyState(VK_F2) & 1)
    GUI::open = !GUI::open;

  // pass messages to imgui
  if (GUI::open &&
      ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
    return 1L;

  return CallWindowProc(GUI::oWindowProc, window, message, wideParam,
                        longParam);
}
