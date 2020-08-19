#pragma once

#include "IL/Renderer/Buffer.h"

namespace IL
{

	enum class RenderAPI
	{
		NONE = 0, OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RenderAPI GetAPI() { return s_RenderAPI; }
	private:
		static RenderAPI s_RenderAPI;
	};

}
