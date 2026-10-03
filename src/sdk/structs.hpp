#pragma once
#include <util/math.hpp>

typedef unsigned short word;

// Other stances
// Walking and Moving: 1024
// Crouching and Moving: 4096
// Proning and Moving: 256
// Going down ladder: 524288
// On ladder / going up ladder: 262144

enum STANCE : uint32_t {
    STANDING = 2,
    CROUCHING = 4,
    PRONING = 8,
    RUNNING = 1048576
};

enum TEAM : uint32_t {
    DEFENSE = 1,
    ATTACK = 2,
    SPECTATOR = 3
};

enum ENTITYTYPE : uint32_t {
    // from uc
    TYPE_SMOKE = 0,
    TYPE_HUMAN = 1,
    TYPE_DEAD = 2,
    TYPE_WEAPON = 3,
    TYPE_EXPLOSIVE = 4,
    TYPE_VEHICLE = 6,
    TYPE_CLAYMORELASER = 8,
    TYPE_TURRET = 11,
    TYPE_HELICOPTER = 12,
    TYPE_OLDEXPLOSIVE = 78,
};

struct CEntity_t {
    char pad_0000[2]; //0x0000
    bool IsValidEntity; //0x0002
    char pad_0003[25]; //0x0003
    Vector3 Position; //0x001C
    Vector2 ViewAngles; //0x0028
    char pad_0030[68]; //0x0030
    Vector3 oldPosition; //0x0074
    char pad_0080[24]; //0x0080
    Vector2 oldViewAngles; //0x0098
    char pad_00A0[44]; //0x00A0
    uint32_t ClientNumber; //0x00CC
    uint32_t EntityType; //0x00D0
    char pad_00D4[16]; //0x00D4
    Vector3 NewPosition; //0x00E4
    char pad_00F0[24]; //0x00F0
    Vector2 NewViewAngles; //0x0108
    char pad_0110[176]; //0x0110
    uint32_t IsAlive; //0x01C0
    char pad_01C4[24]; //0x01C4
}; //Size: 0x01DC
static_assert(sizeof(CEntity_t) == 0x1DC);

struct ClientInfo_t {
    uint32_t IsValidEntity; //0x0000
    char pad_0004[4]; //0x0004
    uint32_t ClientNumber; //0x0008
    char N0000036F[16]; //0x000C
    uint32_t Team; //0x001C
    char pad_0020[28]; //0x0020
    char WeaponClass[32]; //0x003C
    char pad_005C[904]; //0x005C
    Vector2 ViewAngles; //0x03E4
    char pad_03EC[132]; //0x03EC
    uint32_t Stance; //0x0470
    char pad_0474[20]; //0x0474
    uint32_t Shooting; //0x0488
    char pad_048C[4]; //0x048C
    uint32_t ADS; //0x0490
    char pad_0494[24]; //0x0494
    uint32_t WeaponNumber; //0x04AC
    char pad_04B0[28]; //0x04B0
}; //Size: 0x04CC
static_assert(sizeof(ClientInfo_t) == 0x4CC);

// ViewAxis needed for W2S
struct Refdef_t {
    uint32_t x; //0x0000
    uint32_t y; //0x0004
    uint32_t WindowWidth; //0x0008
    uint32_t WindowHeight; //0x000C
    float FovX; //0x0010
    float FovY; //0x0014
    Vector3 ViewOrigin; //0x0018
    Vector3 ViewAxis[3]; //0x0024
    char unknown142[0x4050]; // 0x0048
    Vector3	refdefViewAngles; // 0x4098
}; //Size: 0x40A4
static_assert(sizeof(Refdef_t) == 0x40A4);

struct Camera {
    char pad_0000[24]; //0x0000
    char MapName[16]; //0x0018
    char pad_0028[100]; //0x0028
    float maybeFOV; //0x008C
    char pad_0090[20]; //0x0090
    Vector3 Position; //0x00A4
    char pad_00B0[12]; //0x00B0
    Vector2 ViewAngles; //0x00BC
}; //Size: 0x00C4
static_assert(sizeof(Camera) == 0xC4);