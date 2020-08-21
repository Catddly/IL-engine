#pragma once

#include "RenderCommand.h"

namespace IL
{

	class Renderer
	{
	public:
		// TODO
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& VertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
