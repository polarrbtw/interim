#pragma once
#include <cstdint>
#include <util/math.hpp>
#include <sdk/structs.hpp>

class Entity {
public:
  bool IsValid() const;
  bool IsAlive() const;
  bool IsADS() const;
  bool IsShooting() const;

  uint32_t GetIndex() const;
  uint32_t GetTeam() const;
  uint32_t GetStance() const;
  uint32_t GetEntityType() const;
  uint32_t GetWeaponNumber() const;

  Vector3 GetPosition() const;

  const char* GetName();

  void SetInfo(const CEntity_t& centity, const ClientInfo_t& cinfo, const char* name, const char* clantag, int index);

protected:
  CEntity_t m_centity;
  ClientInfo_t m_cinfo;
  int32_t m_index;
  const char* m_name;
  const char* m_clantag;
};

class LocalPlayer : public Entity {
public:
  float FOV;
  
  Vector2 GetViewAngles() const;

  float GetFOV() const;
  float SetFOV(float newFOV);
};
