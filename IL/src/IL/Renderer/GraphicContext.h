#pragma once

#include <GLFW/glfw3.h>

namespace IL
{

	class GraphicContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}