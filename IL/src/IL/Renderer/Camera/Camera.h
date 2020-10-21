#pragma once

#include <glm/glm.hpp>

namespace IL
{

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			:m_ProjectionMatrix(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};

}