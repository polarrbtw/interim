#pragma once
#include <Windows.h>
#include <cstdio>

class Memory {
public:
  uintptr_t modBase{};
  HMODULE hModule{nullptr};

  // cod4x
  uintptr_t cod4xBase{};
  HMODULE cod4xModule{ nullptr };

  bool Setup() {
    hModule = GetModuleHandleA(NULL);
    modBase = reinterpret_cast<uintptr_t>(hModule);

    cod4xModule = GetModuleHandleA("cod4x_021.dll");
    cod4xBase = reinterpret_cast<uintptr_t>(cod4xModule);

    if (!modBase || !hModule) {
      printf("failed to initialize memory\n");
      return false;
    }

    if (!cod4xModule || !cod4xBase) {
        printf("failed to get base of cod4x_021.dll\n");
        return false;
    }

    printf("[*] iw3mp.exe base: 0x%X\n", modBase);
    printf("[*] cod4x base: 0x%X\n", cod4xBase);
    return true;
  }

  template <typename T> T read(uintptr_t addr) {
    return *reinterpret_cast<T *>(modBase + addr);
  }

  template <typename T> T raw(uintptr_t addr) {
    return *reinterpret_cast<T *>(addr);
  }

  template <typename T> void write(uintptr_t addr, T value) {
    *reinterpret_cast<T *>(modBase + addr) = value;
  }

  template <typename T> void writeRaw(uintptr_t addr, T value) {
      *reinterpret_cast<T*>(addr) = value;
  }

  void NOP(PVOID addr, int bytes) {}

  void Patch(uintptr_t addr, int instructions, int size) {}

  void WriteDvar(uintptr_t dvar, int option) {
      write<BYTE>(dvar, option);
  }

};

inline Memory mem;
