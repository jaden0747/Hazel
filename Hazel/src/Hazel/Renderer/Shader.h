#pragma once

#include <string>
#include <glm/glm.hpp>

namespace hazel
{
  class Shader
  {
  public:
    virtual ~Shader() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    static Shader* create(const std::string& filepath);
    static Shader* create(const std::string& vertexSrc, const std::string& fragmentSrc);
  };
}