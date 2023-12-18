#include "hzpch.h"

#include "Hazel/Core/Application.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace hazel
{

  Application* Application::s_instance = nullptr;

  Application::Application()
  {
    HZ_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;
    m_window = (Window::create());
    m_window->setEventCallback(HZ_BIND_EVENT_FN(Application::onEvent));

    Renderer::init();

    m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
  }

  Application::~Application()
  {
    Renderer::shutdown();
  }


  void Application::pushLayer(Layer* layer) {
    m_layerStack.pushLayer(layer);
  }


  void Application::pushOverlay(Layer* layer) {
    m_layerStack.pushOverlay(layer);
  }


  void Application::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::onWindowClose));
    dispatcher.dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::onWindowResize));
    for (auto it = m_layerStack.end(); it != m_layerStack.begin(); ) {
      (*--it)->onEvent(e);
      if (e.m_handled) {
        break;
      }
    }
  }


  void Application::run() {
    while (m_running) {

      float time = (float)glfwGetTime(); // Platform::GetTime()
      Timestep timestep = time - m_lastFrameTime;
      m_lastFrameTime = time;

      if (!m_minimized)
      {
        for (Layer* layer : m_layerStack) {
          layer->onUpdate(timestep);
        }
      }
      
			m_ImGuiLayer->begin();
			for (Layer* layer : m_layerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->end();

      m_window->onUpdate();
    }
  }


  bool Application::onWindowClose(WindowCloseEvent& e) {
    m_running = false;
    return true;
  }


  bool Application::onWindowResize(WindowResizeEvent& e) {
    if (e.getWidth() == 0 || e.getHeight() == 0)
    {
      m_minimized = true;
      return false;
    }

    m_minimized = false;
    Renderer::onWindowResize(e.getWidth(), e.getHeight());
    return false;
  }

}
