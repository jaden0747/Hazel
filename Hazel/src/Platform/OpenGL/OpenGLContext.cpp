#include "hzpch.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace hazel
{
  OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_windowHandle(windowHandle)
  {
    HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
  }


  void OpenGLContext::init()
  {
    glfwMakeContextCurrent(m_windowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
  }


  void OpenGLContext::swapBuffers()
  {
    glfwSwapBuffers(m_windowHandle);
  }
}
