#include "Entity.hpp"
#include <sdk/offsets.hpp>
#include <util/memory.hpp>

// entity
bool Entity::IsValid() const { return CEntity->IsValidEntity; }
bool Entity::IsAlive() const { return CEntity->IsAlive; }
bool Entity::IsADS() const { return ClientInfo->ADS; }
bool Entity::IsShooting() const { return ClientInfo->Shooting; }

// localplayer
void LocalEntity::SetViewAngles(Vector2 NewViewAngles) {
	uintptr_t ViewAnglesAddr = offsets::ClientInfo + (Index * sizeof(ClientInfo_t)) + 0x03E4; // 0x03E4 is ViewAngles in ClientInfo_t

	Mem.WriteRaw<Vector2>(ViewAnglesAddr, NewViewAngles);
}
