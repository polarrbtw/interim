#pragma once
#include <Windows.h>
#include <cstdio>

class Memory {
public:
  uintptr_t ModBase{};
  HMODULE HModule{nullptr};

  // cod4x
  uintptr_t Cod4xBase{};
  HMODULE Cod4xModule{ nullptr };

  bool Setup() {
    HModule = GetModuleHandleA(NULL);
    ModBase = reinterpret_cast<uintptr_t>(HModule);

    Cod4xModule = GetModuleHandleA("cod4x_021.dll");
    Cod4xBase = reinterpret_cast<uintptr_t>(Cod4xModule);

    if (!ModBase || !HModule) {
      printf("failed to initialize memory\n");
      return false;
    }

    if (!Cod4xModule || !Cod4xBase) {
        printf("failed to get base of cod4x_021.dll\n");
        return false;
    }

    printf("[*] iw3mp.exe base: 0x%X\n", ModBase);
    printf("[*] cod4x base: 0x%X\n", Cod4xBase);
    return true;
  }

  // with iw3mp.exe as base address
  template <typename T> T Read(uintptr_t addr) {
    return *reinterpret_cast<T*>(ModBase + addr);
  }

  // without base address
  template <typename T> T Raw(uintptr_t addr) {
    return *reinterpret_cast<T*>(addr);
  }

  // without dereferencing
  template <typename T> T Ptr(uintptr_t addr) {
    return reinterpret_cast<T>(addr);
  }

  // with iw3mp.exe as base address
  template <typename T> void Write(uintptr_t addr, T value) {
    *reinterpret_cast<T*>(ModBase + addr) = value;
  }

  // without base address
  template <typename T> void WriteRaw(uintptr_t addr, T value) {
    *reinterpret_cast<T*>(addr) = value;
  }

  // todo
  void NOP(PVOID addr, int bytes) {}

  void Patch(uintptr_t addr, int instructions, int size) {}

  void WriteDvar(uintptr_t dvar, int option) {
    Write<BYTE>(dvar, option);
  }

};

inline Memory Mem;
