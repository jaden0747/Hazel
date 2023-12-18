#pragma once

#include "Hazel/Renderer/RenderCommand.h"

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Shader.h"

namespace hazel
{

  class Renderer
  {
  public:
    static void init();
    static void shutdown();
    static void onWindowResize(uint32_t width, uint32_t height);

    static void beginScene(OrthographicCamera& camera);
    static void endScene();

    static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform=glm::mat4(1.0f));

    static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

  private:
    struct SceneData
    {
      glm::mat4 m_viewProjectionMatrix;
    };

    static Scope<SceneData> s_sceneData;
  };
}