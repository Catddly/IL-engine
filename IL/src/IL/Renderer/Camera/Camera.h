#pragma once

#include <glm/glm.hpp>

namespace IL
{

	class Camera
	{
	public:
		enum class CameraType
		{
			Null = 0, Orthographic, Pespective
		};

		virtual ~Camera() {}

		virtual const glm::mat4& GetViewMatrix()		   const = 0;
		virtual const glm::mat4& GetProjectionMatrix()	   const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

		virtual const glm::vec3& GetPosition() const = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual const float& GetRotation() const = 0;
		virtual void SetRotation(const float& rotation) = 0;

		static std::shared_ptr<Camera> CreateDefault(CameraType type);
		static std::shared_ptr<Camera> CreateOrtho(float left, float right, float bottom, float top);
	private:
		virtual void ReCalViewMatrix() = 0;
	};

}