#include <IL.h>

// -------------EntryPoint---------------
#include "IL/Core/EntryPoint.h"
// --------------------------------------
#include "EditorLayer.h"

namespace IL
{
	class ILEditor : public Application
	{
	public: 
		ILEditor() : Application("IL Editor")
		{
			PushLayer(new EditorLayer());
		}
		~ILEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new ILEditor();
	}
}

