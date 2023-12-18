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
  hazel::Renderer2D::resetStats();
  {
    HZ_PROFILE_SCOPE("Renderer Prep");
    hazel::RenderCommand::setClearColor({ 1.0f, 1.0f, 1.0f, 1 });
    hazel::RenderCommand::clear();
  }

  {
    static float rotation = 0.0f;
    rotation += ts * 50.0f;

    HZ_PROFILE_SCOPE("Renderer Draw");
    hazel::Renderer2D::beginScene(m_cameraController.getCamera());
		hazel::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		// hazel::Renderer2D::drawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
    hazel::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		hazel::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		hazel::Renderer2D::drawQuad({ -0.0f, -0.0f, -0.1f }, { 20.0f, 20.0f }, m_checkerboardTexture, 10.0f);
		hazel::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_checkerboardTexture, 20.0f);
    hazel::Renderer2D::endScene();

    hazel::Renderer2D::beginScene(m_cameraController.getCamera());
    for (float y = -5.0f; y < 5.0f; y += 0.5f)
    {
      for (float x = -5.0f; x < 5.0f; x += 0.5f)
      {
        glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
        hazel::Renderer2D::drawRotatedQuad({ x, y }, { 0.45f, 0.45f }, rotation, color);
      }
    }
    hazel::Renderer2D::endScene();
  }
}

void Sandbox2D::onImGuiRender()
{
  HZ_PROFILE_FUNCTION();
  ImGui::Begin("Settings");
	auto stats = hazel::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
  ImGui::End();
}

void Sandbox2D::onEvent(hazel::Event& e)
{
  m_cameraController.onEvent(e);
}
