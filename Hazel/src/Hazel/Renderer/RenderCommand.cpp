#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace hazel
{
  RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}