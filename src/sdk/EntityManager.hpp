#pragma once
#include <vector>
#include "Entity.hpp"

class EntityManager {
public:
  void UpdateEntities();
  void UpdateLocalPlayer();

  const static LocalPlayer* GetLocalPlayer();
  const static std::vector<Entity>& GetEntities();

private:
  inline static std::vector<Entity> entities{};
  inline static LocalPlayer localPlayer{};
};
