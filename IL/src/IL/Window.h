#pragma once

#include "Il/Core.h"
#include "IL/Events/Event.h"

namespace IL
{

	// the properties of the window
	struct WindowProps
	{
		std::string titles;
		UINT width;
		UINT height;

		WindowProps(std::string t = "IL Engine", UINT w = 1280, UINT h = 720)
			:titles(t), width(w), height(h) {}
	};

	// the abstraction of the window that based on every system desktop platform
	class IL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;  // callback function

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual UINT GetWidth() const = 0;
		virtual UINT GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}