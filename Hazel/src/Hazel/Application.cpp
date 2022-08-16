#include "hzpch.h"

#include "Application.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace hazel
{

  #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application::Application() {
    m_window = std::unique_ptr<Window>(Window::create());
    m_window->setEventCallback(BIND_EVENT_FN(onEvent));
  }

  
  Application::~Application() {

  }

  void Application::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
    HZ_CORE_TRACE("{0}", e);
  }


  void Application::run() {
    while (m_running) {
      glClearColor(1, 0, 1, 1);
      // glClear(GL_COLOR_BUFFET_BIT);
      m_window->onUpdate();
    }
  }


  bool Application::onWindowClose(WindowCloseEvent& e) {
    m_running = false;
    return true;
  }

}