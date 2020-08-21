#include "ilpch.h"
#include "VertexArray.h"

#include "IL/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace IL
{

	IL::VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::NONE:
			IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLVertexArray();
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

}