#include "Hazel/Renderer/Shader.h"

#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "hzpch.h"

namespace hazel
{
Ref<Shader> Shader::create(const std::string& filepath)
{
    switch (RendererAPI::getAPI())
    {
    case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return createRef<OpenGLShader>(filepath);
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
    switch (RendererAPI::getAPI())
    {
    case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }

    HZ_CORE_ASSERT(false, "Unkonwn RendererAPI!");
    return nullptr;
}

void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader)
{
    HZ_CORE_ASSERT(!exists(name), "Shader already exists!");
    m_shaders[name] = shader;
}

void ShaderLibrary::add(const Ref<Shader>& shader)
{
    auto& name = shader->getName();
    add(name, shader);
}

Ref<Shader> ShaderLibrary::load(const std::string& filepath)
{
    auto shader = Shader::create(filepath);
    add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
{
    auto shader = Shader::create(filepath);
    add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::get(const std::string& name)
{
    HZ_CORE_ASSERT(exists(name), "Shader not found!");
    return m_shaders[name];
}

bool ShaderLibrary::exists(const std::string& name) const
{
    return m_shaders.find(name) != m_shaders.end();
}
} // namespace hazel
