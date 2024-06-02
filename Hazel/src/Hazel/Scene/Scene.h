#pragma once

#include "entt.hpp"

#include "Hazel/Core/Timestep.h"

namespace hazel
{

class Entity;

class Scene
{
public:
  Scene();
  ~Scene();

  Entity createEntity(const std::string& name = std::string());

  void onUpdate(Timestep ts);
  void onViewportResize(uint32_t width, uint32_t height);

private:
  entt::registry m_registry;
  uint32_t m_viewportWidth = 0;
  uint32_t m_viewportHeight = 0;

  friend class Entity;
};

}
