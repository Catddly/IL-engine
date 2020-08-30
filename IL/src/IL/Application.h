#pragma once

#include "Core.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "IL/Core/TimeStep.h"

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

		inline static Application& GetApplication() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	private:
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		bool m_ShowWindowTrace = false;

		LayerStack  m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		TimeStep timer;
		float m_LastFrameTime;
		
		static Application* s_Instance;  // we only need one application
	};

	// To be define in client
	Application* CreateApplication();
}
