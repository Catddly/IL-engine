#pragma once

#include "IL/Core/Core.h"
#include "IL/Input/KeyCodes.h"

namespace IL
{

	class IL_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);

		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};

}