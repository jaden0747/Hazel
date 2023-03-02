#include "hzpch.h"

#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include <glad/glad.h>

namespace hazel
{
  static bool s_GLFWInitialized = false;


  static void GLFWErrorCallback(int error, const char* description) {
    HZ_CORE_ERROR("GLFW Error ({}): {1}", error, description);
  }


  Window* Window::create(const WindowProps& props) {
    return new WindowsWindow(props);
  }


  WindowsWindow::WindowsWindow(const WindowProps& props) {
    init(props);
  }

  
  WindowsWindow::~WindowsWindow() {
    shutdown();
  }


  void WindowsWindow::init(const WindowProps& props) {
    m_data.title = props.title;
    m_data.width = props.width;
    m_data.height = props.height;

    HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

    if (!s_GLFWInitialized) {
      //todo: glfwTerminate on system shutdown
      int success = glfwInit();
      HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
      glfwSetErrorCallback(GLFWErrorCallback);
      s_GLFWInitialized = true;
    }

    m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    int status = gladLoadGLLoader(((GLADloadproc)glfwGetProcAddress));
    HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
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
          KeyPressedEvent event(key, 0);
          data.eventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          KeyReleasedEvent event(key);
          data.eventCallback(event);
          break;
        }
        case GLFW_REPEAT: {
          KeyPressedEvent event(key, 1);
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
      KeyTypedEvent event(keycode);
      data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      switch (action) {
        case GLFW_PRESS: {
          MouseButtonPressedEvent event(button);
          data.eventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          MouseButtonReleasedEvent event(button);
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
    glfwDestroyWindow(m_window);
  }


  void WindowsWindow::onUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_window);
  }


  void WindowsWindow::setVSync(bool enabled) {
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
