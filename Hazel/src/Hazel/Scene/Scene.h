#pragma once

#include "entt.hpp"

#include "Hazel/Core/Timestep.h"

namespace hazel
{

class Scene
{
public:
  Scene();
  ~Scene();

  entt::entity createEntity();

  entt::registry& reg() { return m_registry; }

  void onUpdate(Timestep ts);

private:
  entt::registry m_registry;
};

}
