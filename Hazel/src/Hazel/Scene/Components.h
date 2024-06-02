#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace hazel
{

struct TagComponent
{
    std::string m_tag;

    TagComponent() = default;
    TagComponent(const TagComponent &) = default;
    TagComponent(const std::string &tag)
        : m_tag(tag)
    {
    }
};

struct TransformComponent
{
    glm::mat4 m_transform{1.0f};

    TransformComponent() = default;
    TransformComponent(const TransformComponent &) = default;
    TransformComponent(const glm::mat4 &transform)
        : m_transform(transform)
    {
    }

    operator glm::mat4 &()
    {
        return m_transform;
    }
    operator const glm::mat4 &() const
    {
        return m_transform;
    }
};

struct SpriteRendererComponent
{
    glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent &) = default;
    SpriteRendererComponent(const glm::vec4 &color)
        : m_color(color)
    {
    }
};

struct CameraComponent
{
    SceneCamera m_camera;
    bool m_primary = true;  // Todo think about moving to Scene
    bool m_fixedAspectRatio = false;

    CameraComponent() = default;
    CameraComponent(const CameraComponent &) = default;
};

struct NativeScriptComponent
{
    ScriptableEntity *m_instance = nullptr;

    std::function<void()> instantiateFunction;
    std::function<void()> destroyInstanceFunction;

    std::function<void(ScriptableEntity *)> onCreateFunction;
    std::function<void(ScriptableEntity *)> onDestroyFunction;
    std::function<void(ScriptableEntity *, Timestep)> onUpdateFunction;

    template <typename T>
    void bind()
    {
        instantiateFunction = [&]() { m_instance = new T(); };
        destroyInstanceFunction = [&]() {
            delete (T *)m_instance;
            m_instance = nullptr;
        };

        onCreateFunction = [](ScriptableEntity *instance) { ((T *)instance)->onCreate(); };
        onDestroyFunction = [](ScriptableEntity *instance) { ((T *)instance)->onDestroy(); };
        onUpdateFunction = [](ScriptableEntity *instance, Timestep ts) { ((T *)instance)->onUpdate(ts); };
    }
};

}  // namespace hazel