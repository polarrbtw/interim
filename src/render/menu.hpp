#pragma once

// menu.cpp
namespace Menu {

    namespace Tabs{
        void CombatTab();
        void VisualsTab();
        void ExploitsTab();
        void DebugTab();

        //inline bool DemoWindow{ false };
    }

    void Render();
}

// widgets.cpp
namespace Widgets {
    bool CheckboxPicker(const char* label, bool *v, const char* p_label, float col[4]);

    enum class NotifyType { Success, Warning, Error, Info };

    void Notify(NotifyType type, const char* msg, int dismissMs = 3000);
    // must be called once per frame on the render thread
    void RenderNotifications();
    // must be called from render thread only
    void ClearNotifications();
}

// short names so you can call like: Widgets::Notify(SUCCESS, "text")
// ERR instead of ERROR
inline constexpr Widgets::NotifyType SUCCESS = Widgets::NotifyType::Success;
inline constexpr Widgets::NotifyType WARNING = Widgets::NotifyType::Warning;
inline constexpr Widgets::NotifyType ERR     = Widgets::NotifyType::Error;
inline constexpr Widgets::NotifyType INFO    = Widgets::NotifyType::Info;
