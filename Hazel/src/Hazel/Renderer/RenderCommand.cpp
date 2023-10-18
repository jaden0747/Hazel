#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace hazel
{
  Scope<RendererAPI> RenderCommand::s_rendererAPI = createScope<OpenGLRendererAPI>();
}