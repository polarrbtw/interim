#pragma once
#include <cstdint>
#include <util/math.hpp>
#include <sdk/structs.hpp>

class Entity {
public:
  CEntity_t* CEntity{};
  ClientInfo_t* ClientInfo{};
  int32_t Index{};
  const char* Name{};
  const char* Clantag{};

  bool IsValid() const;
  bool IsAlive() const;
  bool IsADS() const;
  bool IsShooting() const;
};

class LocalEntity : public Entity {
public:
  float FOV{};

  void SetViewAngles(Vector2 NewViewAngles);
};
