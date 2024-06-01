#include "hzpch.h"
#include "Entity.h"

namespace hazel
{

Entity::Entity(entt::entity handle, Scene* scene)
    : m_entityHandle(handle), m_scene(scene)
{

}

}