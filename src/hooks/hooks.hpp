#pragma once
#include "render/gui.hpp"
#include <globals.hpp>
#include <sdk/EntityManager.hpp>

namespace Hooks {
void Setup();
void Destroy();

constexpr void *VirtualFunction(void *thisptr, size_t index) {
  return (*static_cast<void ***>(thisptr))[index];
}

using EndSceneFn = long(__thiscall *)(void *, IDirect3DDevice9 *);
inline EndSceneFn oEndScene{ nullptr };
long __stdcall EndScene(IDirect3DDevice9 *device);

using ResetFn = HRESULT(__thiscall *)(void *, IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
inline ResetFn oReset{ nullptr };
HRESULT __stdcall Reset(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *p);

// CL_ParseSnapshot
using CL_ParseSnapshotFn = void(__cdecl*)(void*);
inline CL_ParseSnapshotFn oParseSnapshot{ nullptr };
void __cdecl hk_ParseSnapshot(void* msg);

// ScreenshotRequest
using ScreenshotRequestFn = int(__cdecl*)(int, int);
inline ScreenshotRequestFn oScreenshotRequest{ nullptr };
int __cdecl hk_ScreenshotRequest(int msg, int command);

}; // namespace Hooks
