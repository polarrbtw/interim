#include <windows.h>

#include <hooks/hooks.hpp>
#include <thread>

#include <common.hpp>

// features
#include <features/esp.hpp>

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

void RunESP() {
    while (globals->Running) {
        ESP::DrawPlayers();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Setup(const HMODULE instance) {
    try {
        CreateConsole();
        mem.Setup(); // for some reason this was never called??
        GUI::Setup();
        fn::Setup(); // actually initialize but wanted to keep it consistent
        Hooks::Setup();

        std::thread(UpdateGlobals).detach();
    }
    catch (const std::exception& err) {
        MessageBeep(MB_ICONERROR);
        MessageBox(nullptr, err.what(), "interim", MB_OK | MB_ICONEXCLAMATION);

        goto UNLOAD;
    }

    while (!GetAsyncKeyState(VK_END) && globals->Running)
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

UNLOAD:
    globals->Running = false;
    Hooks::Destroy();
    GUI::Destroy();
    DestroyConsole();

    FreeLibraryAndExitThread(instance, 0);
}

BOOL WINAPI DllMain(const HMODULE instance, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);

        const auto thread = CreateThread(
            nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), instance,
            0, nullptr);

        if (thread)
            CloseHandle(thread);
    }

    return true;
}
