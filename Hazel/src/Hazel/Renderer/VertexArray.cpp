#include "hzpch.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace hazel
{
  Ref<VertexArray> VertexArray::create()
  {
    switch (Renderer::getAPI())
    {
      case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL:
        return createRef<OpenGLVertexArray>();
    }
    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }
}