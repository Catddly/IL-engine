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
		virtual ~RendererAPI() = default;

		virtual void SetViewPortSize(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		virtual void Init() = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}