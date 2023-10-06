#include "hzpch.h"
#include "Renderer.h"

namespace hazel
{
  Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

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
    shader->uploadUniformMat4("u_viewProjection", s_sceneData->m_viewProjectionMatrix);
    shader->uploadUniformMat4("u_transform", transform);

    vertexArray->bind();
    RenderCommand::drawIndexed(vertexArray);
  }
}