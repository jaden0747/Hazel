#include "Entity.h"

#include "hzpch.h"

namespace hazel
{

Entity::Entity(entt::entity handle, Scene* scene)
    : m_entityHandle(handle),
      m_scene(scene)
{
}

}  // namespace hazel
