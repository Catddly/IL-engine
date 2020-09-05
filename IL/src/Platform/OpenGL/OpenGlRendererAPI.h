#pragma once

#include "IL/Renderer/RendererAPI.h"

namespace IL
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetViewPortSize(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(glm::vec4 color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

		virtual void Init();
	};

}