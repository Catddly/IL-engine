#pragma once

#include "IL/Renderer/GraphicContext.h"

struct GLFWwindow;

namespace IL
{

	class OpenGLContext : public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}