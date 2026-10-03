#include "math.hpp"
#include <globals.hpp>

bool WorldToScreen(const Vec3& in, Vec2& out)
{
    int ScreenCenterX = globals->refdef.WindowWidth / 2;
    int ScreenCenterY = globals->refdef.WindowHeight / 2;

    Vec3 Right = globals->refdef.ViewAxis[1];
    Vec3 Up = globals->refdef.ViewAxis[2];
    Vec3 Forward = globals->refdef.ViewAxis[0];

    Vec3 Local = in - globals->refdef.ViewOrigin;

    Vec3 TransForm;
    TransForm[0] = (Local[0] * Right[0] + Local[1] * Right[1] + Local[2] * Right[2]);
    TransForm[1] = (Local[0] * Up[0] + Local[1] * Up[1] + Local[2] * Up[2]);
    TransForm[2] = (Local[0] * Forward[0] + Local[1] * Forward[1] + Local[2] * Forward[2]);

    if (TransForm.z < 0.01f)
        return false;

    out.x = ScreenCenterX * (1.0f - (TransForm.x / globals->refdef.FovX / TransForm.z));
    out.y = ScreenCenterY * (1.0f - (TransForm.y / globals->refdef.FovY / TransForm.z));

    return true;
}

// ORIGINAL
/*
bool WorldToScreen(float* WorldLocation, float* ScreenX, float* ScreenY)
{
    int ScreenCenterX = globals->refdef.WindowWidth / 2;
    int ScreenCenterY = globals->refdef.WindowHeight / 2;

    Vec3 Local;
    Vec3 TransForm;
    Vec3 Right = globals->refdef.ViewAxis[1];
    Vec3 Up = globals->refdef.ViewAxis[2];
    Vec3 Forward = globals->refdef.ViewAxis[0];

    // Convert float* WorldLocation into a Vec3 and use overloaded operator-
    Vec3 worldVec(WorldLocation[0], WorldLocation[1], WorldLocation[2]);
    Local = worldVec - globals->refdef.ViewOrigin;

    TransForm[0] = (Local[0] * Right[0] + Local[1] * Right[1] + Local[2] * Right[2]);
    TransForm[1] = (Local[0] * Up[0] + Local[1] * Up[1] + Local[2] * Up[2]);
    TransForm[2] = (Local[0] * Forward[0] + Local[1] * Forward[1] + Local[2] * Forward[2]);

    if (TransForm.z < 0.01)
        return 0;

    if (ScreenX && ScreenY)
    {
        *ScreenX = ScreenCenterX * (1 - (TransForm.x / globals->refdef.FovX / TransForm.z));
        *ScreenY = ScreenCenterY * (1 - (TransForm.y / globals->refdef.FovY / TransForm.z));
    }

    return true;
}
*/