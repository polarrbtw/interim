#pragma once
#include <d3d9.h>
#include <windows.h>

namespace GUI {
inline bool initialized = false;
inline bool open = false; // is menu open

inline HWND hwnd = nullptr;
inline WNDCLASSEX wc{};
inline WNDPROC oWindowProc = nullptr;

// directx
inline LPDIRECT3DDEVICE9 device = nullptr;
inline LPDIRECT3D9 d3d9 = nullptr;

bool SetupWC(const char *wcName);
void DestroyWC();

bool SetupWindow(const char *wndName);
void DestroyWindow();

bool SetupDirectX();
void DestroyDirectX();

void Setup();
void SetupMenu(LPDIRECT3DDEVICE9 device);
void Destroy();

void RenderMenu();

void Render();
}; // namespace GUI
