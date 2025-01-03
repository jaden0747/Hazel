#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "hzpch.h"

namespace hazel
{
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_windowHandle(windowHandle)
{
    HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
}

void OpenGLContext::init()
{
    HZ_PROFILE_FUNCTION();

    glfwMakeContextCurrent(m_windowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

    HZ_CORE_INFO("OpenGL Info:");
    HZ_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    HZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    HZ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef HZ_ENABLE_ASSERTS
    int versionMajor;
    int versionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

    HZ_CORE_ASSERT(
        versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
#endif // HZ_ENABLE_ASSERTS
}

void OpenGLContext::swapBuffers()
{
    HZ_PROFILE_FUNCTION();

    glfwSwapBuffers(m_windowHandle);
}
} // namespace hazel
