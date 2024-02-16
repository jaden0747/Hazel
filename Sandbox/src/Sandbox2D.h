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

  hazel::Ref<hazel::Texture2D> m_checkerboardTexture;

  glm::vec4 m_squareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};
