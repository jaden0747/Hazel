#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "hzpch.h"

namespace hazel
{

SceneCamera::SceneCamera()
{
    recalculateProjection();
}

void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
{
    m_orthographicSize = size;
    m_orthographicNear = nearClip;
    m_orthographicFar  = farClip;
    recalculateProjection();
}

void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
{
    m_aspectRatio = (float)width / (float)height;
    recalculateProjection();
}

void SceneCamera::recalculateProjection()
{
    float orthoLeft   = -m_orthographicSize * m_aspectRatio * 0.5f;
    float orthoRight  = m_orthographicSize * m_aspectRatio * 0.5f;
    float orthoBottom = -m_orthographicSize * 0.5f;
    float orthoTop    = m_orthographicSize * 0.5f;

    m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
}

} // namespace hazel
