#include <Hazel.h>

#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public hazel::Layer {
public:
  ExampleLayer()
  : Layer("Example")
  , m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
  , m_cameraPosition(0.0f)
  {
    m_vertexArray.reset(hazel::VertexArray::create());

    float vertices[3 * 7] = {
      -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
       0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
       0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };

    hazel::Ref<hazel::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(hazel::VertexBuffer::create(vertices, sizeof(vertices)));
    hazel::BufferLayout layout = {
      { hazel::ShaderDataType::Float3, "a_Position" },
      { hazel::ShaderDataType::Float4, "a_Color" }
    };
    vertexBuffer->setLayout(layout);
    m_vertexArray->addVertexBuffer(vertexBuffer);

    uint32_t indices[3] = { 0, 1, 2 };
    hazel::Ref<hazel::IndexBuffer> indexBuffer;
    indexBuffer.reset(hazel::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_vertexArray->setIndexBuffer(indexBuffer);

    m_squareVertexArray.reset(hazel::VertexArray::create());

    float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    };

    hazel::Ref<hazel::VertexBuffer> squareVertexBuffer;
    squareVertexBuffer.reset(hazel::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
    squareVertexBuffer->setLayout({
      { hazel::ShaderDataType::Float3, "a_Position" },
      { hazel::ShaderDataType::Float2, "a_TexCoord" }
    });
    m_squareVertexArray->addVertexBuffer(squareVertexBuffer);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    hazel::Ref<hazel::IndexBuffer> squareIndexBuffer;
    squareIndexBuffer.reset(hazel::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_squareVertexArray->setIndexBuffer(squareIndexBuffer);

    std::string vertexSrc = R"glsl(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_viewProjection * u_transform * vec4(a_Position, 1.0);	
			}
		)glsl";

		std::string fragmentSrc = R"glsl(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				// color = vec4(v_Position * 0.5, 1.0);
				color = v_Color;
			}
		)glsl";

		m_shader.reset(hazel::Shader::create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"glsl(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_viewProjection * u_transform * vec4(a_Position, 1.0);	
			}
		)glsl";

		std::string flatColorShaderFragmentSrc = R"glsl(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

      uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)glsl";

    m_textureShader.reset(hazel::Shader::create("Sandbox/assets/shaders/Texture.glsl"));

    m_texture = hazel::Texture2D::create("Sandbox/assets/textures/Checkerboard.png");

    std::dynamic_pointer_cast<hazel::OpenGLShader>(m_textureShader)->bind();
    std::dynamic_pointer_cast<hazel::OpenGLShader>(m_textureShader)->uploadUniformInt("u_Texture", 0);

		m_flatColorShader.reset(hazel::Shader::create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
  }
  


  void onUpdate(hazel::Timestep ts) override {
    if (hazel::Input::isKeyPressed(HZ_KEY_LEFT))
      m_cameraPosition.x -= m_cameraMoveSpeed * ts;
    else if (hazel::Input::isKeyPressed(HZ_KEY_RIGHT))
      m_cameraPosition.x += m_cameraMoveSpeed * ts;

    if (hazel::Input::isKeyPressed(HZ_KEY_UP))
      m_cameraPosition.y += m_cameraMoveSpeed * ts;
    else if (hazel::Input::isKeyPressed(HZ_KEY_DOWN))
      m_cameraPosition.y -= m_cameraMoveSpeed * ts;

    if (hazel::Input::isKeyPressed(HZ_KEY_A))
      m_cameraRotation += m_cameraRotationSpeed * ts;
    else if (hazel::Input::isKeyPressed(HZ_KEY_D))
      m_cameraRotation -= m_cameraRotationSpeed * ts;
    
    hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    hazel::RenderCommand::clear();

    m_camera.setPosition(m_cameraPosition);
    m_camera.setRotation(m_cameraRotation);

    hazel::Renderer::beginScene(m_camera);

    std::dynamic_pointer_cast<hazel::OpenGLShader>(m_flatColorShader)->bind();
    std::dynamic_pointer_cast<hazel::OpenGLShader>(m_flatColorShader)->uploadUniformFloat3("u_Color", m_squareColor);
    
    for (int y = 0; y < 20; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        hazel::Renderer::submit(m_flatColorShader, m_vertexArray, transform);
      }
    }

    m_texture->bind();
    hazel::Renderer::submit(m_textureShader, m_squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(0.7f)));

    // hazel::Renderer::submit(m_shader, m_vertexArray);

    hazel::Renderer::endScene();
  }

  virtual void onImGuiRender() override
	{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
    ImGui::End();
	}


  void onEvent(hazel::Event& event) override 
  {
  #ifdef WIN32
    // HZ_INFO("{0}: {1}", __FUNCTION__, event.getName());
  #else
    // HZ_INFO("{0}: {1}", __func__, event.getName());
  #endif
  }

private:
  hazel::Ref<hazel::Shader> m_shader;
  hazel::Ref<hazel::VertexArray> m_vertexArray;
  
  hazel::Ref<hazel::Shader> m_flatColorShader, m_textureShader;
  hazel::Ref<hazel::VertexArray> m_squareVertexArray;

  hazel::Ref<hazel::Texture2D> m_texture;

  hazel::OrthographicCamera m_camera;
  glm::vec3 m_cameraPosition;
  float m_cameraMoveSpeed = 5.0f;

  float m_cameraRotation = 0.0f;
  float m_cameraRotationSpeed = 180.0f;

  glm::vec3 m_squareColor = {0.2f, 0.3f, 0.8f};
};


class Sandbox : public hazel::Application
{
public:
  Sandbox() {
    pushLayer(new ExampleLayer());
  }


  ~Sandbox() {
    
  }


};


hazel::Application* hazel::createApplication() {
  return new Sandbox();
}