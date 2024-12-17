#include "RenderCommand.h"

#include "Hazel/Renderer/RenderCommand.h"
#include "hzpch.h"

namespace hazel
{
Scope<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::create();
}
