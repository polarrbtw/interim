#include "Entity.hpp"

// entity
bool Entity::IsValid() const { return m_centity.IsValidEntity; }
bool Entity::IsAlive() const { return m_centity.IsAlive; }
bool Entity::IsADS() const { return m_cinfo.ADS; }
bool Entity::IsShooting() const { return m_cinfo.Shooting; }

uint32_t Entity::GetIndex() const { return m_centity.ClientNumber; }
uint32_t Entity::GetTeam() const { return m_cinfo.Team; }
uint32_t Entity::GetStance() const { return m_cinfo.Stance; }
uint32_t Entity::GetEntityType() const { return m_centity.EntityType; }
uint32_t Entity::GetWeaponNumber() const { return m_cinfo.WeaponNumber; }

Vector3 Entity::GetPosition() const { return m_centity.Position; };

const char* Entity::GetName() { return m_name; }

void Entity::SetInfo(const CEntity_t& centity, const ClientInfo_t& cinfo, const char* name, const char* clantag, int index) {
	m_centity = centity;
	m_cinfo = cinfo;
	m_index = index;
	m_name = name;
	m_clantag = clantag;
}

// localplayer
Vector2 LocalPlayer::GetViewAngles() const { return m_centity.ViewAngles; }
float LocalPlayer::GetFOV() const { return FOV; }
float LocalPlayer::SetFOV(float newFOV) { FOV = newFOV; }