#pragma once

#include "RendererAPI.h"

namespace IL
{ 

	class RenderCommand
	{
	public:
		// dispatch the renderCommand to the API which we are using
		// RenderCommand do not do multiple things
		static void SetViewPortSize(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewPortSize(x, y, width, height);
		}

		static void SetClearColor(glm::vec4 color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		static void Init()
		{
			s_RendererAPI->Init();
		}
	private:
		static RendererAPI* s_RendererAPI;
	};

}