#include "Hazel/Renderer/OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "hzpch.h"

namespace hazel
{

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_aspectRatio(aspectRatio)
    , m_camera({-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel})
    , m_rotation(rotation)
{
}

void OrthographicCameraController::onUpdate(Timestep ts)
{
    HZ_PROFILE_FUNCTION();

    if (Input::isKeyPressed(Key::A))
    {
        m_cameraPosition.x -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
        m_cameraPosition.y -= sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
    }
    else if (Input::isKeyPressed(Key::D))
    {
        m_cameraPosition.x += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
        m_cameraPosition.y += sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
    }

    if (Input::isKeyPressed(Key::W))
    {
        m_cameraPosition.x += -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
        m_cameraPosition.y += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
    }
    else if (Input::isKeyPressed(Key::S))
    {
        m_cameraPosition.x -= -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
        m_cameraPosition.y -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
    }

    if (m_rotation)
    {
        if (Input::isKeyPressed(Key::Q))
        {
            m_cameraRotation += m_cameraRotationSpeed * ts;
        }
        if (Input::isKeyPressed(Key::E))
        {
            m_cameraRotation -= m_cameraRotationSpeed * ts;
        }

        if (m_cameraRotation > 180.0f)
        {
            m_cameraRotation -= 360.0f;
        }
        else if (m_cameraRotation <= -180.0f)
        {
            m_cameraRotation += 360.0f;
        }

        m_camera.setRotation(m_cameraRotation);
    }

    m_camera.setPosition(m_cameraPosition);

    m_cameraTranslationSpeed = m_zoomLevel;
}

void OrthographicCameraController::onEvent(Event& e)
{
    HZ_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
    dispatcher.dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
}

void OrthographicCameraController::onResize(float width, float height)
{
    m_aspectRatio = width / height;
    m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
}

bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
{
    HZ_PROFILE_FUNCTION();

    m_zoomLevel -= e.getOffsetY() * 0.25f;
    m_zoomLevel = std::max(m_zoomLevel, 0.25f);
    m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
    return false;
}

bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
{
    HZ_PROFILE_FUNCTION();

    onResize((float)e.getWidth(), (float)e.getHeight());
    return false;
}

} // namespace hazel
