#include "ilpch.h"
#include "Camera.h"

#include "IL/Renderer/Camera/OrthographicCamera.h"

namespace IL
{

	std::shared_ptr<Camera> Camera::CreateDefault(CameraType type)
	{
		switch (type)
		{
		case IL::Camera::CameraType::Null:
			IL_CORE_ASSERT(false, "Camera can't be null!"); return nullptr;
		case IL::Camera::CameraType::Orthographic:
			return std::make_shared<OrthographicCamera>(-1.0f, 1.0f, -1.0f, 1.0f);
		case IL::Camera::CameraType::Pespective:
			IL_CORE_ASSERT(false, "Camera can't be null!"); return nullptr;
		}

		IL_CORE_ASSERT(false, "Unknown camera type!");
		return nullptr;
	}

	std::shared_ptr<Camera> Camera::CreateOrtho(float left, float right, float bottom, float top)
	{
		return std::make_shared<OrthographicCamera>(left, right, bottom, top);
	}

}
