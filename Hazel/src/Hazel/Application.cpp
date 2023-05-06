#include "hzpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

#include <glad/glad.h>

namespace hazel
{

  #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application* Application::s_instance = nullptr;

  static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type)
    {
			case hazel::ShaderDataType::Float:    return GL_FLOAT;
			case hazel::ShaderDataType::Float2:   return GL_FLOAT;
			case hazel::ShaderDataType::Float3:   return GL_FLOAT;
			case hazel::ShaderDataType::Float4:   return GL_FLOAT;
			case hazel::ShaderDataType::Mat3:     return GL_FLOAT;
			case hazel::ShaderDataType::Mat4:     return GL_FLOAT;
			case hazel::ShaderDataType::Int:      return GL_INT;
			case hazel::ShaderDataType::Int2:     return GL_INT;
			case hazel::ShaderDataType::Int3:     return GL_INT;
			case hazel::ShaderDataType::Int4:     return GL_INT;
			case hazel::ShaderDataType::Bool:     return GL_BOOL;
    }
		
		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
  }

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

    float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

    m_vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

    {
      BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" }
      };
      m_vertexBuffer->setLayout(layout);
    }

    uint32_t index = 0;
    const auto& layout = m_vertexBuffer->getLayout();
    for (const auto& element : layout)
    {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index,
        element.getComponentCount(),
        shaderDataTypeToOpenGLBaseType(element.m_type),
        element.m_normalized ? GL_TRUE : GL_FALSE,
        layout.getStride(),
        (const void*)element.m_offset);
      index++;
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);

    unsigned int indices[3] = { 0, 1, 2 };
    m_indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_shader.reset(new Shader(vertexSrc, fragmentSrc));
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

      m_shader->bind();

      m_vertexBuffer->bind();
      m_indexBuffer->bind();

      glBindVertexArray(m_vertexArray);
      glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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