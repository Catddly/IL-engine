#include "ilpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGlRendererAPI.h"

namespace IL
{

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}