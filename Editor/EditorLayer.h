#pragma once

#include "Hazel.h"

namespace hazel
{

class EditorLayer : public Layer
{
public:
  EditorLayer();
  virtual ~EditorLayer() = default;

  void onAttach() override;
  void onDetach() override;

  void onUpdate(Timestep ts) override;
  void onImGuiRender() override;
  void onEvent(Event &e) override;

private:
  hazel::OrthographicCameraController m_cameraController;

  Ref<VertexArray> m_squareVA;
  Ref<Shader> m_flatColorShader;
  Ref<Framebuffer> m_framebuffer;

  Ref<Texture2D> m_checkerboardTexture;

  glm::vec2 m_viewportSize = {0.0f, 0.0f};

  glm::vec4 m_squareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};

}