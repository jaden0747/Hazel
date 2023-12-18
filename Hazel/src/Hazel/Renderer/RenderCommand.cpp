#include "hzpch.h"
#include "RenderCommand.h"

#include "Hazel/Renderer/RenderCommand.h"

namespace hazel
{
  Scope<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::create();
}