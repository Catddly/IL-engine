#pragma once

#include "IL/Layer.h"

#include "IL/Events/KeyEvent.h"
#include "IL/Events/MouseEvent.h"
#include "IL/Events/ApplicationEvent.h"

namespace IL
{

	class IL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDeatch();
		void OnUpdate();
		void OnEvent(Event& e);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);

		bool OnWindowResizedEvent(WindowResizeEvent& e);

		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnKeyReleasedEvent(KeyReleaseEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);

	private:
		float m_Time = 0.0f;
	};

}