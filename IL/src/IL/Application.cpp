#include "ilpch.h"

#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace IL													
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		IL_TRACE(e);
		AppRenderEvent render;
		IL_WARN(render);

		while (true);
	}
}