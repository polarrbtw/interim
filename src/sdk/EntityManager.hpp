#pragma once
#include <vector>
#include "Entity.hpp"

class EntityManager {
public:
  void UpdateEntities();
  void UpdateLocalPlayer();

  static LocalEntity* GetLocalPlayer();
  const static std::vector<Entity>& GetEntities();

private:
  inline static std::vector<Entity> EntityList{};
  inline static LocalEntity LocalPlayer{};
};
