#include "aimbot.hpp"
#include "hooks/engine.hpp"
#include "imgui.h"
#include "sdk/offsets.hpp"
#include "util/math.hpp"
#include <settings.hpp>
#include <common.hpp>
#include <WinUser.h>
#include <algorithm>

namespace {
    float NormalizeAngle(float angle) {
        if (angle > 180.f)  angle -= 360.f;
        else if (angle < -180.f) angle += 360.f;
        return angle;
    }
}

Vector2 Aimbot::CalculateAngles(Vector3 from, Vector3 to) {
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float dz = to.z - from.z;

    float yaw = atan2f(dy, dx) * RADTODEG;
    float pitch = -atan2f(dz, sqrtf(dx * dx + dy * dy)) * RADTODEG;

    return Vector2(yaw, std::clamp(pitch, -90.0f, 90.0f));
}

void Aimbot::RunAimbot() {
    if (!Settings::Aimbot::Enabled) return;

    auto* local = EntityManager::GetLocalPlayer();
    if (!local || !local->CEntity || !local->IsValid()) return;

    if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
        TargetIndex = -1;
        return;
    }

    // TargetIndex is for sticky aimbot (if we have a target to not switch to another one)
    if (TargetIndex != -1 && !TargetEntity.IsAlive())
        TargetIndex = -1;

    // get target
    if (TargetIndex == -1) {
        Entity target{};
        if (!Engine::GetClosestEnemyFOV(target, Settings::Aimbot::MaxDistance))
            return;

        TargetEntity = target;
        TargetIndex  = target.Index;
    }

    Refdef_t* Refdef = reinterpret_cast<Refdef_t*>(offsets::Refdef);

    Vector2 Angles = CalculateAngles(Refdef->ViewOrigin, Engine::GetTagPosition(bones.list[HEAD], TargetEntity.CEntity));

    // some claude math going on here
    const Vector3& ForwardVec = Refdef->ViewAxis[0];
    float CurrentYaw =  atan2f(ForwardVec.y, ForwardVec.x) * RADTODEG;
    float CurrentPitch = -asinf(std::clamp(ForwardVec.z, -1.0f, 1.0f)) * RADTODEG;

    ViewAngles_t* ViewAngles = reinterpret_cast<ViewAngles_t*>(offsets::ViewAngles);
    ViewAngles->pitch += NormalizeAngle(Angles.y - CurrentPitch);
    ViewAngles->yaw += NormalizeAngle(Angles.x - CurrentYaw);
}

void Aimbot::DrawFOVCircle() {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    draw->AddCircle(GetScreenCenter(), (Settings::Aimbot::FOV), WHITE, 0, 1.0f);
}
