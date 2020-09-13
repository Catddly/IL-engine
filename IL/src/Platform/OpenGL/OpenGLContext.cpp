#include "ilpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace IL
{

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:m_WindowHandle(window)
	{
		IL_CORE_ASSERT(m_WindowHandle, "OpenglContext's Handle is null!");
	}

	void OpenGLContext::Init()
	{
		IL_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IL_CORE_ASSERT(status, "Failed to initialize glad!");

		IL_CORE_INFO("OpenGL Info:");
		IL_CORE_INFO("  Vendor : {0}", glGetString(GL_VENDOR));
		IL_CORE_INFO("  Device : {0}", glGetString(GL_RENDERER));
		IL_CORE_INFO("  Version : {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		IL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}