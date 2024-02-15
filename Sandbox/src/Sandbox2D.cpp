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
  static bool dockingEnabled = true;

  if (dockingEnabled)
  {
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Dockspace Demo", &dockspaceOpen, window_flags);

    ImGui::PopStyleVar();
    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    // Dockspace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    //! MenuBar
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Exit")) hazel::Application::get().close();
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    //! Settings
    {
      ImGui::Begin("Settings");
      auto stats = hazel::Renderer2D::getStats();
      ImGui::Text("Renderer2D Stats:");
      ImGui::Text("Draw Calls: %d", stats.drawCalls);
      ImGui::Text("Quads: %d", stats.quadCount);
      ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
      ImGui::Text("Indices: %d", stats.getTotalIndexCount());
      ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
      uint32_t textureID = m_checkerboardTexture->getRendererID();
      ImGui::Image((ImTextureID*)textureID, ImVec2{ 256.0f, 256.0f });
      ImGui::End();
    }
    ImGui::End();
  }
  else
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
		uint32_t textureID = m_checkerboardTexture->getRendererID();
		ImGui::Image((ImTextureID*)textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();
  }
}

void Sandbox2D::onEvent(hazel::Event& e)
{
  m_cameraController.onEvent(e);
}
