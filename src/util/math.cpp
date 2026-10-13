#include "math.hpp"
#include <globals.hpp>

bool WorldToScreen(const Vec3& in, Vec2& out)
{
    int ScreenCenterX = globals->Refdef.WindowWidth / 2;
    int ScreenCenterY = globals->Refdef.WindowHeight / 2;

    Vec3 Right = globals->Refdef.ViewAxis[1];
    Vec3 Up = globals->Refdef.ViewAxis[2];
    Vec3 Forward = globals->Refdef.ViewAxis[0];

    Vec3 Local = in - globals->Refdef.ViewOrigin;

    Vec3 TransForm;
    TransForm.x = (Local.x * Right.x + Local.y * Right.y + Local.z * Right.z);
    TransForm.y = (Local.x * Up.x + Local.y * Up.y + Local.z * Up.z);
    TransForm.z = (Local.x * Forward.x + Local.y * Forward.y + Local.z * Forward.z);

    if (TransForm.z < 0.01f)
        return false;

    out.x = ScreenCenterX * (1.0f - (TransForm.x / globals->Refdef.FovX / TransForm.z));
    out.y = ScreenCenterY * (1.0f - (TransForm.y / globals->Refdef.FovY / TransForm.z));

    return true;
}