#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace hazel
{


struct Renderer2DStorage
{
  Ref<VertexArray> quadVertexArray;
  Ref<Shader> flatColorShader;
};


static Renderer2DStorage* s_data;


void Renderer2D::init()
{
  s_data = new Renderer2DStorage();
  s_data->quadVertexArray = VertexArray::create();

  float squareVertices[3 * 4] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
  };
  

  Ref<VertexBuffer> squareVB;

  squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
  squareVB->setLayout({
    { ShaderDataType::Float3, "a_Position" }
  });

  s_data->quadVertexArray->addVertexBuffer(squareVB);

  uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
  Ref<IndexBuffer> squareIB;
  squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

  s_data->quadVertexArray->setIndexBuffer(squareIB);

  s_data->flatColorShader = Shader::create("Sandbox/assets/shaders/FlatColor.glsl");
}


void Renderer2D::shutdown()
{
  delete s_data;
}


void Renderer2D::beginScene(const OrthographicCamera& camera)
{
  std::dynamic_pointer_cast<OpenGLShader>(s_data->flatColorShader)->bind();
  std::dynamic_pointer_cast<OpenGLShader>(s_data->flatColorShader)->uploadUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix());
  std::dynamic_pointer_cast<OpenGLShader>(s_data->flatColorShader)->uploadUniformMat4("u_Transform", glm::mat4(1.0f));
}


void Renderer2D::endScene()
{

}


void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
  drawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
  std::dynamic_pointer_cast<OpenGLShader>(s_data->flatColorShader)->bind();
  std::dynamic_pointer_cast<OpenGLShader>(s_data->flatColorShader)->uploadUniformFloat4("u_Color", color);

  s_data->quadVertexArray->bind();
  RenderCommand::drawIndexed(s_data->quadVertexArray);
}

} // namespace hazel

