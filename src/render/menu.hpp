#pragma once
// todo: proper menu later

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
}
