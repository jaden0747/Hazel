#include "Hazel/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "hzpch.h"

namespace hazel
{
OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
    , m_viewMatrix(1.0f)
{
    HZ_PROFILE_FUNCTION();

    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
{
    HZ_PROFILE_FUNCTION();

    m_projectionMatrix     = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void OrthographicCamera::recalculateViewMatrix()
{
    HZ_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

    m_viewMatrix           = glm::inverse(transform);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}
} // namespace hazel
