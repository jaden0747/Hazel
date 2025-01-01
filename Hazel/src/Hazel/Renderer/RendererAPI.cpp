#include "Hazel/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "hzpch.h"

namespace hazel
{
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::create()
{
    switch (s_API)
    {
    case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return createScope<OpenGLRendererAPI>();
    }

    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
} // namespace hazel
