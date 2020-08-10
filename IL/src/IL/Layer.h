#pragma once

#include "IL/Core.h"
#include "IL/Events/Event.h"

namespace IL
{

	class IL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDeatch() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

}