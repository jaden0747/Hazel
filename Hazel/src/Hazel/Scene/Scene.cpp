#include "Scene.h"

#include <glm/glm.hpp>

#include "Components.h"
#include "Entity.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "hzpch.h"

namespace hazel
{

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::createEntity(const std::string& name)
{
    Entity entity = {m_registry.create(), this};
    entity.addComponent<TransformComponent>();
    auto& tag = entity.addComponent<TagComponent>();
    tag.m_tag = name.empty() ? "Entity" : name;
    return entity;
}

void Scene::onUpdate(Timestep ts)
{
    // Update scripts
    {
        m_registry.view<NativeScriptComponent>().each(
            [=](auto entity, auto& nsc)
            {
                if (!nsc.m_instance)
                {
                    nsc.m_instance           = nsc.instantiateScript();
                    nsc.m_instance->m_entity = Entity{entity, this};
                    nsc.m_instance->onCreate();
                }
                nsc.m_instance->onUpdate(ts);
            });
    }
    // Render 2D
    Camera*    mainCamera      = nullptr;
    glm::mat4* cameraTransform = nullptr;
    {
        auto view = m_registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.m_primary)
            {
                mainCamera      = &camera.m_camera;
                cameraTransform = &transform.m_transform;
                break;
            }
        }
    }
    if (mainCamera)
    {
        Renderer2D::beginScene(mainCamera->getProjection(), *cameraTransform);

        auto group = m_registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::drawQuad(transform, sprite.m_color);
        }

        Renderer2D::endScene();
    }
}

void Scene::onViewportResize(uint32_t width, uint32_t height)
{
    m_viewportWidth  = width;
    m_viewportHeight = height;

    // Resize our non-fixedAspectRatio cameras
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view)
    {
        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.m_fixedAspectRatio)
        {
            cameraComponent.m_camera.setViewportSize(width, height);
        }
    }
}

} // namespace hazel
