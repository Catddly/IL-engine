#pragma once

#include "IL/Window.h"

#include <GLFW/glfw3.h>

namespace IL
{

	// window on Windows
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline UINT GetWidth() const override  { return m_Data.width; }
		inline UINT GetHeight() const override { return m_Data.height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enable) override;
		bool IsVSync() const override { return m_Data.VSync; }

	private:
		virtual void Init(const WindowProps& props);  // initialize the window
		virtual void Shutdown();                      // close the window
	private:
		GLFWwindow* m_Window;
		
		struct WindowData
		{
			std::string title;
			UINT width, height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}