#pragma once

#include "IL/Layer.h"

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
		float m_Time = 0.0f;
	};

}