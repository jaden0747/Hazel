#include "hzpch.h"

#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

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


		if (m_registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_registry.get<TransformComponent>(entity);


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


entt::entity Scene::createEntity()
{
  return m_registry.create();
}


void Scene::onUpdate(Timestep ts)
{
	// Render 2D
	auto group = m_registry.view<TransformComponent, SpriteRendererComponent>();
	for (auto entity : group)
	{
		TransformComponent& transform = group.get<TransformComponent>(entity);
		SpriteRendererComponent& sprite = group.get<SpriteRendererComponent>(entity);

		Renderer2D::drawQuad(transform, sprite.m_color);
	}

}

}
