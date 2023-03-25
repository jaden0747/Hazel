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

    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

    float vertices[3 * 3] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);

    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glBindVertexArray(m_vertexArray);
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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