#include "hzpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h> // access to OpenGL
#include <glm/gtc/type_ptr.hpp>

namespace hazel
{
  Shader* Shader::create(const std::string& filepath)
  {
    switch (RendererAPI::getAPI())
    {
      case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL:  return new OpenGLShader(filepath);
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }

  Shader* Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    switch (RendererAPI::getAPI())
    {
      case RendererAPI::API::None:   HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
    }

    HZ_CORE_ASSERT(false, "Unkonwn RendererAPI!");
    return nullptr;
  }
}