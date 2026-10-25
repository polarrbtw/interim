// todo: proper menu
#include "menu.hpp"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <render/gui.hpp>
#include <globals.hpp>
#include <settings.hpp>

constexpr ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
constexpr ImGuiChildFlags ChildFlags = ImGuiChildFlags_Borders;
constexpr ImGuiColorEditFlags PickerFlags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;

// forward declarations
void ApplyColorStyle();
void Menu::Tabs::CombatTab();
void Menu::Tabs::VisualsTab();
void Menu::Tabs::ExploitsTab();
void Menu::Tabs::DebugTab();

void Menu::Render() {
    ApplyColorStyle();

    ImGui::SetNextWindowSize({640, 260}, ImGuiCond_Once);

    ImGui::Begin("interim", &GUI::open, WindowFlags);
    if (ImGui::BeginTabBar("MyTabBar", 0))
    {
        if (ImGui::BeginTabItem("Combat"))
        {
            Tabs::CombatTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Visuals"))
        {
            Tabs::VisualsTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Exploits"))
        {
            Tabs::ExploitsTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Debug"))
        {
            Tabs::DebugTab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Menu::Tabs::CombatTab() {
    // Aimbot Child
    ImGui::BeginChild("Aimbot", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 195), ChildFlags, WindowFlags); {
        ImGui::Checkbox("Aimbot", &Settings::Aimbot::Enabled);
        ImGui::Checkbox("FOV Circle", &Settings::Aimbot::FOVCircle);
        ImGui::SliderFloat("FOV", &Settings::Aimbot::FOV, 10, 200, "%.0f");
        ImGui::SliderInt("Max Distance", &Settings::Aimbot::MaxDistance, 10, 150, "%dm");
    } ImGui::EndChild();

    ImGui::SameLine();

    // Silent Aim Child (not implemented)
    ImGui::BeginChild("SilentAim", ImVec2(ImGui::GetContentRegionAvail().x, 195), ChildFlags, WindowFlags);  {
        ImGui::Checkbox("Silent Aim", &Settings::SilentAim::Enabled);
        ImGui::Checkbox("FOV Circle", &Settings::SilentAim::FOVCircle);
        ImGui::SliderFloat("FOV", &Settings::SilentAim::FOV, 10, 200, "%.0f");
        ImGui::SliderInt("Max Distance", &Settings::SilentAim::MaxDistance, 10, 150, "%dm");

        ImGui::Text("NOT IMPLEMENTED YET / NON FUNCTIONAL");
    } ImGui::EndChild();
}

void Menu::Tabs::VisualsTab() {
    // Visauls Tab
    ImGui::BeginChild("Visuals", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 195), ChildFlags, WindowFlags); {
        ImGui::Checkbox("ESP", &Settings::ESP::Enabled);
        Widgets::CheckboxPicker("Box", &Settings::ESP::Box, "BoxColPicker", Settings::ESP::BoxColor);
        Widgets::CheckboxPicker("Skeleton", &Settings::ESP::Bone, "SkeletonColPicker", Settings::ESP::BoneColor);
        Widgets::CheckboxPicker("Names", &Settings::ESP::Name, "NameColPicker", Settings::ESP::NameColor);
        Widgets::CheckboxPicker("Distance", &Settings::ESP::Distance, "DistanceColPicker", Settings::ESP::DistanceColor);
        ImGui::Text("Stroke");
        ImGui::SameLine();
        ImGui::ColorEdit4("StrokeColPicker", Settings::ESP::StrokeColor, PickerFlags);

        ImGui::Checkbox("Ignore Dead", &Settings::ESP::IgnoreDead);
        ImGui::Checkbox("Ignore Team", &Settings::ESP::IgnoreTeam);
    } ImGui::EndChild();
}

void Menu::Tabs::ExploitsTab() {
    // Exploits Tab
    ImGui::BeginChild("Exploits", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), ChildFlags, WindowFlags); {
        if (ImGui::Checkbox("Third Person", &Settings::Cvar::ThirdPerson)) {
            if (Settings::Cvar::ThirdPerson)
                Mem.WriteDvar(cvars::cg_thirdperson, 1);
            else
                Mem.WriteDvar(cvars::cg_thirdperson, 0);
        }

        if (ImGui::Checkbox("No Smoke", &Settings::Cvar::NoSmoke)) {
            if (Settings::Cvar::NoSmoke)
                Mem.WriteDvar(cvars::fx_draw, 0);
            else
                Mem.WriteDvar(cvars::fx_draw, 1);
        }

        if (ImGui::Checkbox("No Jump Cooldown", &Settings::Cvar::NoJumpCooldown)) {
            if (Settings::Cvar::NoJumpCooldown)
                Mem.WriteDvar(cvars::jump_slowdown, 0);
            else
                Mem.WriteDvar(cvars::jump_slowdown, 1);
        }
    } ImGui::EndChild();
}

void Menu::Tabs::DebugTab() {
    // Debug Tab
    ImGui::BeginChild("Debug", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 195), ChildFlags, WindowFlags); {
        //ImGui::ShowDemoWindow(0);
        if (ImGui::Button("Unload")) { globals->Running = false; }

        ImGui::Text("iw3mp.exe base: 0x%X", Mem.ModBase);
        ImGui::Text("cod4x_021.dll base: 0x%X", Mem.Cod4xBase);

        #ifdef _DEBUG
            ImGui::SliderFloat("Width", &Settings::Debug::Width, 10.0f, 80.0f, "%.3f", 0);
            ImGui::SliderFloat("Height", &Settings::Debug::Height, 50.0f, 125.0f, "%.3f", 0);

            // rendering thread is NOT fried
            ImGui::ShowMetricsWindow();
        #endif
    } ImGui::EndChild();
}

void ApplyColorStyle() {
    // gray color style
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->WindowBorderSize = 1.0f;
	style->ChildBorderSize = 1.0f;
	style->PopupBorderSize = 1.0f;
	style->FrameBorderSize = 1.0f;

	style->WindowRounding = 2.0f;
	style->ChildRounding = 2.0f;
	style->FrameRounding = 2.0f;
	style->PopupRounding = 2.0f;
	style->GrabRounding = 2.0f;

	colors[ImGuiCol_Text]                   = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	colors[ImGuiCol_Button]                 = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.18f, 0.18f, 0.18f, 0.00f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.22f, 0.22f, 0.22f, 0.78f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
	colors[ImGuiCol_Separator]              = ImVec4(0.29f, 0.29f, 0.29f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.49f, 0.49f, 0.49f, 0.78f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TabSelected]            = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TabDimmed]              = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
	colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.39f, 0.39f, 0.39f, 0.78f);
	colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.29f, 0.29f, 0.29f, 0.50f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextLink]               = ImVec4(0.29f, 0.50f, 1.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavCursor]              = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
