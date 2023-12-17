#pragma once

#include <Hazel.h>

class Sandbox2D : public hazel::Layer
{
public:
  Sandbox2D();
  virtual ~Sandbox2D() = default;

  virtual void onAttach() override;
  virtual void onDetach() override;

  void onUpdate(hazel::Timestep ts) override;
  virtual void onImGuiRender() override;
  void onEvent(hazel::Event& e) override;

private:
  hazel::OrthographicCameraController m_cameraController;

  // Temp
  hazel::Ref<hazel::VertexArray> m_squareVA;
  hazel::Ref<hazel::Shader> m_flatColorShader;
  hazel::Ref<hazel::Shader> m_textureShader;
  hazel::Ref<hazel::Texture2D> m_texture;

  glm::vec4 m_squareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};
