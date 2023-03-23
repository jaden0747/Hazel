#include "hzpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

#include <glad/glad.h>

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

    m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
  }

  
  Application::~Application() {

  }


  void Application::pushLayer(Layer* layer) {
    m_layerStack.pushLayer(layer);
    layer->onAttach();
  }


  void Application::pushOverlay(Layer* layer) {
    m_layerStack.pushOverlay(layer);
    layer->onAttach();
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
      glClearColor(1, 0, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      for (Layer* layer : m_layerStack) {
        layer->onUpdate();
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