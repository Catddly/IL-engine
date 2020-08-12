#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "LayerStack.h"

namespace IL
{

	class IL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);  // use this function to dispatch event to window

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
<<<<<<< HEAD

		inline static Application& GetApplication() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
=======
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		bool m_ShowWindowTrace = false;

		LayerStack m_LayerStack;
<<<<<<< HEAD
	private:
		static Application* s_Instance;  // we only need one application
=======
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
	};

	// To be define in client
	Application* CreateApplication();
}
