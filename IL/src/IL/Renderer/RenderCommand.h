#pragma once

#include "RendererAPI.h"

namespace IL
{ 

	class RenderCommand
	{
	public:
		// dispatch the renderCommand to the API which we are using
		// RenderCommand do not do multiple things
		inline static void SetViewPortSize(UINT width, UINT height)
		{
			s_RendererAPI->SetViewPortSize(width, height);
		}

		inline static void SetClearColor(glm::vec4 color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void Init()
		{
			s_RendererAPI->Init();
		}
	private:
		static RendererAPI* s_RendererAPI;
	};

}