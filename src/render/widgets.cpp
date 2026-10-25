#include "menu.hpp"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <settings.hpp>

bool Widgets::CheckboxPicker(const char* label, bool *v, const char* p_label, float col[4]) {
    constexpr ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs;
    bool state = false;

    if (ImGui::Checkbox(label, v)) {
        state = true;
    }

    ImGui::SameLine();

    if (ImGui::ColorEdit4(p_label, col, flags)) {
        state = true;
    }

    return state;
}
