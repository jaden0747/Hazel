#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
  : Layer("Sandbox2D")
  , m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::onAttach()
{
  HZ_PROFILE_FUNCTION();

  m_checkerboardTexture = hazel::Texture2D::create("Sandbox/assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach()
{
  HZ_PROFILE_FUNCTION();

}

void Sandbox2D::onUpdate(hazel::Timestep ts)
{
  HZ_PROFILE_FUNCTION();
  // update
  m_cameraController.onUpdate(ts);


  // render
  {
    HZ_PROFILE_SCOPE("Renderer Prep");
    hazel::RenderCommand::setClearColor({ 1.0f, 1.0f, 1.0f, 1 });
    hazel::RenderCommand::clear();
  }

  {
    HZ_PROFILE_SCOPE("Renderer Draw");
    hazel::Renderer2D::beginScene(m_cameraController.getCamera());
		// hazel::Renderer2D::drawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
    hazel::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		hazel::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		// hazel::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture, 10.0f);
    hazel::Renderer2D::endScene();
  }
}

void Sandbox2D::onImGuiRender()
{
  HZ_PROFILE_FUNCTION();
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
  ImGui::End();
}

void Sandbox2D::onEvent(hazel::Event& e)
{
  m_cameraController.onEvent(e);
}
