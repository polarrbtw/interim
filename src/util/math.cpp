#include "math.hpp"
#include <globals.hpp>

bool WorldToScreen(const Vec3& in, Vec2& out)
{
    Refdef_t Refdef = Mem.Raw<Refdef_t>(offsets::Refdef);

    int ScreenCenterX = Refdef.WindowWidth / 2;
    int ScreenCenterY = Refdef.WindowHeight / 2;

    Vec3 Right = Refdef.ViewAxis[1];
    Vec3 Up = Refdef.ViewAxis[2];
    Vec3 Forward = Refdef.ViewAxis[0];

    Vec3 Local = in - Refdef.ViewOrigin;

    Vec3 TransForm;
    TransForm.x = (Local.x * Right.x + Local.y * Right.y + Local.z * Right.z);
    TransForm.y = (Local.x * Up.x + Local.y * Up.y + Local.z * Up.z);
    TransForm.z = (Local.x * Forward.x + Local.y * Forward.y + Local.z * Forward.z);

    if (TransForm.z < 0.01f)
        return false;

    out.x = ScreenCenterX * (1.0f - (TransForm.x / Refdef.FovX / TransForm.z));
    out.y = ScreenCenterY * (1.0f - (TransForm.y / Refdef.FovY / TransForm.z));

    if (out.IsZero()) {
        return false;
    }

    return true;
}

Vec2 GetScreenCenter() {
    Refdef_t Refdef = Mem.Raw<Refdef_t>(offsets::Refdef);

    return { Refdef.WindowWidth * 0.5f, Refdef.WindowHeight * 0.5f }; // * 0.5f instead of / 2
}
