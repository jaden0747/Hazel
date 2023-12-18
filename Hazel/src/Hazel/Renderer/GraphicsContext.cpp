#include "hzpch.h"

#include "Hazel/Renderer/GraphicsContext.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace hazel
{
  Scope<GraphicsContext> GraphicsContext::create(void* window)
  {
    switch (Renderer::getAPI())
    {
      case RendererAPI::API::None:   HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL: return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }
}
