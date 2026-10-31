#include "menu.hpp"
#include "notifications/ImGuiNotify.hpp"

#include "imgui.h"

#include <settings.hpp>

#include <mutex>
#include <string>
#include <vector>

namespace {
    struct Pending {
        Widgets::NotifyType type;
        std::string msg;
        int dismissMs;
    };

    // written from any thread, drained on the render thread
    std::mutex g_mutex;
    std::vector<Pending> g_pending;

    constexpr size_t MAX_QUEUED = 32;

    ImGuiToastType ToToastType(Widgets::NotifyType type) {
        switch (type) {
            case Widgets::NotifyType::Success: return ImGuiToastType::Success;
            case Widgets::NotifyType::Warning: return ImGuiToastType::Warning;
            case Widgets::NotifyType::Error:   return ImGuiToastType::Error;
            default:                           return ImGuiToastType::Info;
        }
    }

    const char* TitleFor(Widgets::NotifyType type) {
        switch (type) {
            case Widgets::NotifyType::Warning: return "interim - Warning";
            case Widgets::NotifyType::Error:   return "interim - Error";
            default:                           return "interim";
        }
    }
}

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

void Widgets::Notify(NotifyType type, const char* msg, int dismissMs) {
    if (!msg) return;

    std::lock_guard lk(g_mutex);
    if (g_pending.size() >= MAX_QUEUED)
        g_pending.erase(g_pending.begin());
    g_pending.push_back({ type, msg, dismissMs });
}

void Widgets::RenderNotifications() {
    // anything pushed since the last frame, taken off the queue in one shot
    std::vector<Pending> incoming;
    {
        std::lock_guard lk(g_mutex);
        incoming.swap(g_pending);
    }

    for (auto& p : incoming) {
        ImGuiToast toast(ToToastType(p.type), p.dismissMs);
        toast.setTitle("%s", TitleFor(p.type));
        toast.setContent("%s", p.msg.c_str());
        ImGui::InsertNotification(toast);
    }

    // same palette as ApplyColorStyle() in menu.cpp
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.18f, 0.18f, 0.18f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.22f, 0.22f, 0.22f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.69f, 0.69f, 0.69f, 1.f));

    ImGui::RenderNotifications();

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
}

void Widgets::ClearNotifications() {
    std::lock_guard lk(g_mutex);
    g_pending.clear();
    ImGui::notifications.clear();
}
