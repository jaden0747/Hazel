#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hazel
{

EditorLayer::EditorLayer()
  : Layer("EditorLayer")
  , m_cameraController(1920.0f / 1080.0f, true)
  , m_squareColor({0.2f, 0.3f, 0.8f, 1.0f})
{
}


void EditorLayer::onAttach()
{
  HZ_PROFILE_FUNCTION();
  m_checkerboardTexture = hazel::Texture2D::create("resources/assets/textures/Checkerboard.png");

  hazel::FramebufferSpecification fbSpec;
  fbSpec.width = 1920;
  fbSpec.height = 1080;
  m_framebuffer = hazel::Framebuffer::create(fbSpec);
}


void EditorLayer::onDetach()
{
  HZ_PROFILE_FUNCTION();
}

void EditorLayer::onUpdate(hazel::Timestep ts)
{
  HZ_PROFILE_FUNCTION();

  // update
  m_cameraController.onUpdate(ts);

  // render
  hazel::Renderer2D::resetStats();
  {
    HZ_PROFILE_SCOPE("Renderer Prep");
    m_framebuffer->bind();
    hazel::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
    hazel::RenderCommand::clear();
  }

  {
    static float rotation = 0.0f;
    rotation += ts * 50.0f;

    HZ_PROFILE_SCOPE("Renderer Draw");
    hazel::Renderer2D::beginScene(m_cameraController.getCamera());
    hazel::Renderer2D::drawRotatedQuad({1.0f, 0.0f, 0.1f}, {0.8f, 0.8f}, glm::radians(rotation), m_squareColor);
    hazel::Renderer2D::drawQuad({-1.0f, 0.0f, 0.1f}, {0.8f, 0.8f}, m_squareColor);
    hazel::Renderer2D::drawQuad({0.5f, -0.5f, 0.1f}, {0.5f, 0.75f}, m_squareColor);
    hazel::Renderer2D::drawQuad({0.0f, 0.0f, 0.1f}, {20.0f, 20.0f}, m_checkerboardTexture, 10.0f);
    hazel::Renderer2D::endScene();

    hazel::Renderer2D::beginScene(m_cameraController.getCamera());
    for (float y = -5.0f; y < 5.0f; y+=0.5f)
    {
      for (float x = -5.0f; x < 5.0f; x+=0.5f)
      {
        glm::vec4 color = {0.8f + 0.1f * x, 0.2f, 0.8f + 0.1f * y, 1.0f};
        hazel::Renderer2D::drawQuad({x, y, -0.1f}, {0.45f, 0.45f}, color);
      }
    }
    hazel::Renderer2D::endScene();
    m_framebuffer->unbind();
  }
}


void EditorLayer::onImGuiRender()
{
  HZ_PROFILE_FUNCTION();

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
    {
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

      if (ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("File"))
        {
          if (ImGui::MenuItem("Exit")) Application::get().close();
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      ImGui::Begin("Settings");
      {
        auto stats = hazel::Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

        uint32_t textureID = m_framebuffer->getColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{1920, 1080}, ImVec2{0, 1}, ImVec2{1, 0});
      }
      ImGui::End();
    }
    ImGui::End();
  }
  else
  {
    ImGui::Begin("Settings");
    {
      auto stats = hazel::Renderer2D::getStats();
      ImGui::Text("Renderer2D Stats:");
      ImGui::Text("Draw Calls: %d", stats.drawCalls);
      ImGui::Text("Quads: %d", stats.quadCount);
      ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
      ImGui::Text("Indices: %d", stats.getTotalIndexCount());

      ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
    }
    ImGui::End();
  }
}


void EditorLayer::onEvent(hazel::Event& e)
{
  m_cameraController.onEvent(e);
}

}