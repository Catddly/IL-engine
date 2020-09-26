#include "ilpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace IL
{

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProps& props)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLFrameBuffer>(props);
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

}