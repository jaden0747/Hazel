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

    HZ_CORE_INFO("OpenGL Info:");
    HZ_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    HZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    HZ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    
  }


  void OpenGLContext::swapBuffers()
  {
    glfwSwapBuffers(m_windowHandle);
  }
}
