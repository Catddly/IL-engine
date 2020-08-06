#include "ilpch.h"
#include "WindowsWindow.h"

namespace IL
{

	static bool m_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.title  = props.titles;
		m_Data.width  = props.width;
		m_Data.height = props.height;
		
		IL_CORE_INFO("Create window {0}, ({1}, {2})", props.titles, props.width, props.height);

		if (!m_GLFWInitialized)
		{
			// TODO : glfwTerminate on system shutdown
			int success = glfwInit();
			IL_CORE_ASSERT(success, "Could not initialize GLFW!");

			m_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.width, (int)props.height, props.titles.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		if (enable)
			glfwSwapInterval(1);
		else 
			glfwSwapInterval(0);

		m_Data.VSync = enable;
	}
}
