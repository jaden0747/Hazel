#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace hazel
{

class OrthographicCameraController
{
public:
  OrthographicCameraController(float aspectRatio, bool rotation = false);

  void onUpdate(Timestep ts);
  void onEvent(Event& e);

  OrthographicCamera& getCamera() { return m_camera; }
  const OrthographicCamera& getCamera() const { return m_camera; }

private:
  bool onMouseScrolled(MouseScrolledEvent& e);
  bool onWindowResized(WindowResizeEvent& e);

private:
  float m_aspectRatio;
  float m_zoomLevel = 1.0f;
  OrthographicCamera m_camera;

  bool m_rotation;

  glm::vec3 m_cameraPosition = {0.0f, 0.0f, 0.0f};
  float m_cameraRotation = 0.0f;
  float m_cameraTranslationSpeed = 5.0f;
  float m_cameraRotationSpeed = 180.0f;

};

} // namespace hazel
