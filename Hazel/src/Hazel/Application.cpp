#include "hzpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"
#include "Hazel/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace hazel
{

  #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application* Application::s_instance = nullptr;

  Application::Application()
  {
    HZ_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;
    m_window = std::unique_ptr<Window>(Window::create());
    m_window->setEventCallback(BIND_EVENT_FN(onEvent));

    Renderer::init();

    m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
  }


  void Application::pushLayer(Layer* layer) {
    m_layerStack.pushLayer(layer);
  }


  void Application::pushOverlay(Layer* layer) {
    m_layerStack.pushOverlay(layer);
  }


  void Application::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
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

      for (Layer* layer : m_layerStack) {
        layer->onUpdate(timestep);
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

}