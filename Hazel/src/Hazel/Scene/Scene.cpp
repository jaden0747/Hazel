#include "Scene.h"

#include <glm/glm.hpp>

#include "Components.h"
#include "Entity.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "hzpch.h"

namespace hazel
{

static void doMath(const glm::mat4& transform)
{
}

static void onTransformConstruct(entt::registry& registry, entt::entity entity)
{
}

Scene::Scene()
{
#define ENTT_EXAMPLE_CODE 0
#if ENTT_EXAMPLE_CODE

    entt::entity entity = m_registry.create();
    m_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

    m_registry.on_construct<TransformComponent>().connect<&onTransformConstruct>();

    if (m_registry.has<TransformComponent>(entity)) TransformComponent& transform = m_registry.get<TransformComponent>(entity);

    auto view = m_registry.view<TransformComponent>();
    for (auto entity : view)
    {
        TransformComponent& transform = view.get<TransformComponent>(entity);
    }

    // auto group = m_registry.group<TransformComponent>(entt::get<MeshComponent>);
    // for (auto entity : group)
    // {
    // 	auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
    // }

#endif
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
        m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
            if (!nsc.m_instance)
            {
                nsc.instantiateFunction();
                nsc.m_instance->m_entity = Entity{entity, this};

                if (nsc.onCreateFunction)
                {
                    nsc.onCreateFunction(nsc.m_instance);
                }
            }

            if (nsc.onUpdateFunction)
            {
                nsc.onUpdateFunction(nsc.m_instance, ts);
            }
        });
    }
    // Render 2D
    Camera* mainCamera = nullptr;
    glm::mat4* cameraTransform = nullptr;
    {
        auto view = m_registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.m_primary)
            {
                mainCamera = &camera.m_camera;
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
            TransformComponent& transform = group.get<TransformComponent>(entity);
            SpriteRendererComponent& sprite = group.get<SpriteRendererComponent>(entity);

            Renderer2D::drawQuad(transform, sprite.m_color);
        }

        Renderer2D::endScene();
    }
}

void Scene::onViewportResize(uint32_t width, uint32_t height)
{
    m_viewportWidth = width;
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

}  // namespace hazel
