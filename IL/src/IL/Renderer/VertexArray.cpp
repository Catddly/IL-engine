#include "ilpch.h"
#include "VertexArray.h"

#include "IL/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace IL
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

}