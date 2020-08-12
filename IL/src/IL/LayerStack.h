#pragma once

#include "IL/Core.h"
#include "Layer.h"

#include <vector>

namespace IL
{

	class IL_API LayerStack
	{
	public:
		typedef std::vector<Layer*> Layers;

		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		Layers::iterator begin() { return m_Layers.begin(); }
		Layers::iterator end()   { return m_Layers.end(); }

	private:
		Layers m_Layers;
		Layers::iterator m_LayerInsert;
	};

}