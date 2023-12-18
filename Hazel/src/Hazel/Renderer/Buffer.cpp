#include "hzpch.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace hazel
{
  Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
  {
    switch (Renderer::getAPI())
    {
      case RendererAPI::API::None:   HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL: return  createRef<OpenGLVertexBuffer>(vertices, size);
    }
    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }

  Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size)
  {
    switch (Renderer::getAPI())
    {
      case RendererAPI::API::None:   HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL: return  createRef<OpenGLIndexBuffer>(indices, size);
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }
}