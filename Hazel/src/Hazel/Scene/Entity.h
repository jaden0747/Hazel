#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace hazel
{

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template <typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        HZ_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
        return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
    }

    template <typename T>
    T& getComponent()
    {
        HZ_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        return m_scene->m_registry.get<T>(m_entityHandle);
    }

    template <typename T>
    bool hasComponent()
    {
        return m_scene->m_registry.try_get<T>(m_entityHandle) != nullptr;
    }

    template <typename T>
    void removeComponent()
    {
        HZ_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        m_scene->m_registry.remove<T>(m_entityHandle);
    }

    operator bool() const
    {
        return m_entityHandle != entt::null;
    }

    operator uint32_t() const
    {
        return (uint32_t)m_entityHandle;
    }

    bool operator==(const Entity& other) const
    {
        return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
    }

    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

private:
    entt::entity m_entityHandle{entt::null};
    Scene*       m_scene = nullptr;
};

} // namespace hazel
