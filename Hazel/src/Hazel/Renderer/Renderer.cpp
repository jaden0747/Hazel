#include "hzpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace hazel
{
  Scope<Renderer::SceneData> Renderer::s_sceneData = createScope<Renderer::SceneData>();

  void Renderer::init()
  {
    RenderCommand::init();
  }

  void Renderer::onWindowResize(uint32_t width, uint32_t height)
  {
    RenderCommand::setViewport(0, 0, width, height);
  }
   
  void Renderer::beginScene(OrthographicCamera& camera)
  {
    s_sceneData->m_viewProjectionMatrix = camera.getViewProjectionMatrix();
  }

  void Renderer::endScene()
  {

  }

  void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
  {
    shader->bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", s_sceneData->m_viewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Transform", transform);

    vertexArray->bind();
    RenderCommand::drawIndexed(vertexArray);
  }
}