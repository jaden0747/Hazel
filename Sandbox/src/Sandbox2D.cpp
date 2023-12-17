#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
  : Layer("Sandbox2D")
  , m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::onAttach()
{
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(hazel::Timestep ts)
{
  // update
  m_cameraController.onUpdate(ts);

  // render
  hazel::RenderCommand::setClearColor({ 1.0f, 1.0f, 1.0f, 1 });
  hazel::RenderCommand::clear();

  hazel::Renderer2D::beginScene(m_cameraController.getCamera());
  hazel::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_squareColor);
  hazel::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender()
{
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
  ImGui::End();
}

void Sandbox2D::onEvent(hazel::Event& e)
{
  m_cameraController.onEvent(e);
}
