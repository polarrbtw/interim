#include <features/aimbot.hpp>
#include <render/gui.hpp>
#include <render/menu.hpp>
#include <windows.h>

#include <hooks/hooks.hpp>
#include <features/esp.hpp>
#include <features/exploits.hpp>
#include <thread>

#include <common.hpp>

// util
void CreateConsole();
void DestroyConsole();

// threads
void UpdateGlobals();
void AimbotThread();

// initialization and unloading
[[noreturn]] void Unload(const HMODULE instance) {
    globals->Running = false;
    if (Hooks::initialized)
        Hooks::Destroy();
    if (GUI::initialized)
        GUI::Destroy();
    Exploits::Destroy(); // disable all cvars

    DestroyConsole();
    FreeLibraryAndExitThread(instance, 0);
}

void Setup(const HMODULE instance) {
    try {
        CreateConsole();
        Mem.Setup();
        GUI::Setup();
        Engine::Setup(); // actually initialize but wanted to keep it consistent
        Hooks::Setup();

        // start threads
        std::thread(UpdateGlobals).detach();
        std::thread(AimbotThread).detach();
        // esp runs on the render thread

        Widgets::Notify(SUCCESS, "interim started correctly!");
    }
    catch (const std::exception& err) {
        MessageBeep(MB_ICONERROR);
        MessageBox(nullptr, err.what(), "interim", MB_OK | MB_ICONEXCLAMATION);

        Unload(instance);
    }

    while (!GetAsyncKeyState(VK_END) && globals->Running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    Unload(instance);
}

BOOL WINAPI DllMain(const HMODULE instance, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);

        const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), instance, 0, nullptr);

        if (thread) CloseHandle(thread);
    }

    return true;
}

void CreateConsole() {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);
    SetConsoleTitleA("debug");
}

void DestroyConsole() {
    fclose(stdout);
    fclose(stderr);
    fclose(stdin);
    FreeConsole();
}

void UpdateGlobals() {
    while (globals->Running) {
        globals->Update();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void AimbotThread() {
    while (globals->Running) {
        Aimbot::RunAimbot();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
