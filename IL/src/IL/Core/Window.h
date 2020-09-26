#pragma once

#include "IL/Core/Core.h"
#include "IL/Events/Event.h"

namespace IL
{

	// the properties of the window
	struct WindowProps
	{
		std::string titles;
		uint32_t width;
		uint32_t height;

		WindowProps(std::string t = "IL Engine", uint32_t w = 1280, uint32_t h = 720)
			:titles(t), width(w), height(h) {}
	};

	// the abstraction of the window that based on every system desktop platform
	class IL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;  // callback function

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}