#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
  : Layer("Sandbox2D")
  , m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::onAttach()
{
  m_squareVA = hazel::VertexArray::create();

  float squareVertices[4 * 3] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
  };

  hazel::Ref<hazel::VertexBuffer> squareVB;
  squareVB.reset(hazel::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
  squareVB->setLayout({
    { hazel::ShaderDataType::Float3, "a_Position" },
    // { hazel::ShaderDataType::Float2, "a_TexCoord"}
  });
  m_squareVA->addVertexBuffer(squareVB);

  uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
  hazel::Ref<hazel::IndexBuffer> squareIB;
  squareIB.reset(hazel::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
  m_squareVA->setIndexBuffer(squareIB);

  m_texture = hazel::Texture2D::create("Sandbox/assets/textures/ChernoLogo.png");

  m_flatColorShader = (hazel::Shader::create("Sandbox/assets/shaders/FlatColor.glsl"));
  m_textureShader = (hazel::Shader::create("Sandbox/assets/shaders/Texture.glsl"));

  std::dynamic_pointer_cast<hazel::OpenGLShader>(m_textureShader)->bind();
  std::dynamic_pointer_cast<hazel::OpenGLShader>(m_textureShader)->uploadUniformInt("u_Texture", 0);
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(hazel::Timestep ts)
{
  // update
  m_cameraController.onUpdate(ts);

  // render
  hazel::RenderCommand::setClearColor({ 1.0f, 1.0f, 1.0f, 1 });
  hazel::RenderCommand::clear();

  hazel::Renderer::beginScene(m_cameraController.getCamera());


  // m_texture->bind();
  // hazel::Renderer::submit(m_textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
  std::dynamic_pointer_cast<hazel::OpenGLShader>(m_flatColorShader)->bind();
  std::dynamic_pointer_cast<hazel::OpenGLShader>(m_flatColorShader)->uploadUniformFloat4("u_Color", m_squareColor);

  hazel::Renderer::submit(m_flatColorShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

  hazel::Renderer::endScene();
}

void Sandbox2D::onImGuiRender()
{
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
  ImGui::End();
}

void Sandbox2D::onEvent(hazel::Event& e)
{
  m_cameraController.onEvent(e);
}
