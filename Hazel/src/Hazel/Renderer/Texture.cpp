#include "hzpch.h"

#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace hazel
{

Ref<Texture2D> Texture2D::create(const std::string& path)
{
  switch (Renderer::getAPI())
  {
    case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
    case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
  }

  HZ_CORE_ASSERT(false, "Unknown RendererAPI!");

  return nullptr;
}

} // namespace hazel
