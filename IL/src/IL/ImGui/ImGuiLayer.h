#pragma once

#include "IL/Layer/Layer.h"

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

		virtual void OnAttach() override;
		virtual void OnDeatch() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:
		float m_Time = 0.0f;
	};

}