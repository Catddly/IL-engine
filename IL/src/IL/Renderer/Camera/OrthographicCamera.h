#pragma once

#include <glm/glm.hpp>

namespace IL
{

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjectionMatrix(float left, float right, float bottom, float top);

		const glm::mat4& GetViewMatrix()	       const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix()	   const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectMatrix; }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; ReCalViewMatrix(); }

		const float& GetRotation() const { return m_Rotation; }
		void SetRotation(const float& rotation) { m_Rotation = rotation; ReCalViewMatrix(); }
	private:
		void ReCalViewMatrix();
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectMatrix;

		float     m_Rotation;
		glm::vec3 m_Position;
	};

}