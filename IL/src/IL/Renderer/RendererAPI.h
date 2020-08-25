#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace IL
{

	class RendererAPI // interface
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1 
		};
	public:
		virtual void SetViewPortSize(UINT width, UINT height) = 0;
		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}