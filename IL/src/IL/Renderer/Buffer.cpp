#include "ilpch.h"
#include "Buffer.h"

#include "IL/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace IL
{

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::NONE:
			IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::NONE:
			IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

}