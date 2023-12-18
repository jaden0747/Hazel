#include "hzpch.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "Hazel/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace hazel
{
  static uint8_t s_GLFWWindowCount = 0u;


  static void GLFWErrorCallback(int error, const char* description) {
    HZ_CORE_ERROR("GLFW Error ({}): {1}", error, description);
  }


  Scope<Window> Window::create(const WindowProps& props) {
    return createScope<WindowsWindow>(props);
  }


  WindowsWindow::WindowsWindow(const WindowProps& props) {
    HZ_PROFILE_FUNCTION();
  
    init(props);
  }

  WindowsWindow::~WindowsWindow() {
    HZ_PROFILE_FUNCTION();
  
    shutdown();
  }


  void WindowsWindow::init(const WindowProps& props) {
    HZ_PROFILE_FUNCTION();
  
    m_data.title = props.title;
    m_data.width = props.width;
    m_data.height = props.height;

    HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

    if (s_GLFWWindowCount == 0u) {
      HZ_PROFILE_SCOPE("glfwInit");
      //todo: glfwTerminate on system shutdown
      int success = glfwInit();
      HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
      glfwSetErrorCallback(GLFWErrorCallback);
    }
    {
      HZ_PROFILE_SCOPE("glfwCreateWindow");
    #if defined(DEBUG)
      if (Renderer::getAPI() == RendererAPI::API::OpenGL)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif
      m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
      ++s_GLFWWindowCount;
    }
    
    m_context = GraphicsContext::create(m_window);
    m_context->init();

    glfwSetWindowUserPointer(m_window, &m_data);
    setVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      data.width = width;
      data.height = height;
      WindowResizeEvent event(width, height);
      data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      WindowCloseEvent event;
      data.eventCallback(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      switch (action) {
        case GLFW_PRESS: {
          KeyPressedEvent event(static_cast<KeyCode>(key), 0);
          data.eventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          KeyReleasedEvent event(static_cast<KeyCode>(key));
          data.eventCallback(event);
          break;
        }
        case GLFW_REPEAT: {
          KeyPressedEvent event(static_cast<KeyCode>(key), 1);
          data.eventCallback(event);
          break;
        }
        default: {
          break;
        }
      }
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
    {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      KeyTypedEvent event(static_cast<KeyCode>(keycode));
      data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      switch (action) {
        case GLFW_PRESS: {
          MouseButtonPressedEvent event(static_cast<MouseCode>(button));
          data.eventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
          data.eventCallback(event);
          break;
        }
        default: {
          break;
        }
      }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x_offset, double y_offset) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      MouseScrolledEvent event((float)x_offset, (float)y_offset);
      data.eventCallback(event);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x_pos, double y_pos) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      MouseMovedEvent event((float)x_pos, (float)y_pos);
      data.eventCallback(event);
    });
  }  


  void WindowsWindow::shutdown() {
    HZ_PROFILE_FUNCTION();
  
    glfwDestroyWindow(m_window);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0u)
    {
      glfwTerminate();
    }
  }


  void WindowsWindow::onUpdate() {
    HZ_PROFILE_FUNCTION();
  
    glfwPollEvents();
    m_context->swapBuffers();
  }


  void WindowsWindow::setVSync(bool enabled) {
    HZ_PROFILE_FUNCTION();
  
    if (enabled) {
      glfwSwapInterval(1);
    } else {
      glfwSwapInterval(0);
    }

    m_data.VSync = enabled;
  }


  bool WindowsWindow::isVSync() const {
    return m_data.VSync;
  }


}
