#include "hzpch.h"

#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace hazel
{


struct Renderer2DStorage
{
  Ref<VertexArray> quadVertexArray;
  Ref<Shader> flatColorShader;
  Ref<Shader> textureShader;
  Ref<Texture2D> whiteTexture;
};


static Renderer2DStorage* s_data;


void Renderer2D::init()
{
  HZ_PROFILE_FUNCTION();

  s_data = new Renderer2DStorage();
  s_data->quadVertexArray = VertexArray::create();

  float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
  };


  Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
  squareVB->setLayout({
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float2, "a_TexCoord" }
  });

  s_data->quadVertexArray->addVertexBuffer(squareVB);

  uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
  Ref<IndexBuffer> squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

  s_data->quadVertexArray->setIndexBuffer(squareIB);

  s_data->whiteTexture = Texture2D::create(1, 1);
  uint32_t whiteTextureData = 0xffffffff;
  s_data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

  s_data->textureShader = Shader::create("Sandbox/assets/shaders/Texture.glsl");
  s_data->textureShader->bind();
  s_data->textureShader->setInt("u_Texture", 0);
}


void Renderer2D::shutdown()
{
  HZ_PROFILE_FUNCTION();

  delete s_data;
}


void Renderer2D::beginScene(const OrthographicCamera& camera)
{
  HZ_PROFILE_FUNCTION();

  s_data->textureShader->bind();
  s_data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
}


void Renderer2D::endScene()
{
  HZ_PROFILE_FUNCTION();


}


void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
  drawQuad({ position.x, position.y, 0.0f }, size, color);
}


void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
  HZ_PROFILE_FUNCTION();

  s_data->textureShader->setFloat4("u_Color", color);
  s_data->textureShader->setFloat("u_TilingFactor", 1.0f);
  s_data->whiteTexture->bind();

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
  s_data->textureShader->setMat4("u_Transform", transform);

  s_data->quadVertexArray->bind();
  RenderCommand::drawIndexed(s_data->quadVertexArray);
}


void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
  drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
}


void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
  HZ_PROFILE_FUNCTION();

  s_data->textureShader->bind();
  s_data->textureShader->setFloat4("u_Color", tintColor);
  s_data->textureShader->setFloat("u_TilingFactor", tilingFactor);
  texture->bind();

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
  s_data->textureShader->setMat4("u_Transform", transform);

  s_data->quadVertexArray->bind();
  RenderCommand::drawIndexed(s_data->quadVertexArray);
}

void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
  drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
  HZ_PROFILE_FUNCTION();

  s_data->textureShader->setFloat4("u_Color", color);
  s_data->textureShader->setFloat("u_TilingFactor", 1.0f);
  s_data->whiteTexture->bind();

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
    * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
    * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
  s_data->textureShader->setMat4("u_Transform", transform);

  s_data->quadVertexArray->bind();
  RenderCommand::drawIndexed(s_data->quadVertexArray);
}

void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
  drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
}

void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
  HZ_PROFILE_FUNCTION();

  s_data->textureShader->bind();
  s_data->textureShader->setFloat4("u_Color", tintColor);
  s_data->textureShader->setFloat("u_TilingFactor", tilingFactor);
  texture->bind();

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
    * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
    * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
  s_data->textureShader->setMat4("u_Transform", transform);

  s_data->quadVertexArray->bind();
  RenderCommand::drawIndexed(s_data->quadVertexArray);
}

} // namespace hazel

