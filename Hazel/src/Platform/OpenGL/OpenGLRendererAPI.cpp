#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "hzpch.h"

namespace hazel
{
void OpenGLMessageCallback(
    unsigned    source,
    unsigned    type,
    unsigned    id,
    unsigned    severity,
    int         length,
    const char* message,
    const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        HZ_CORE_CRITICAL(message);
        return;
    case GL_DEBUG_SEVERITY_MEDIUM:
        HZ_CORE_ERROR(message);
        return;
    case GL_DEBUG_SEVERITY_LOW:
        HZ_CORE_WARN(message);
        return;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        HZ_CORE_TRACE(message);
        return;
    }

    HZ_CORE_ASSERT(false, "Unknown severity level!");
}

void OpenGLRendererAPI::init()
{
    HZ_PROFILE_FUNCTION();

#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
{
    uint32_t count = (indexCount) ? indexCount : vertexArray->getIndexBuffer()->getCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace hazel
