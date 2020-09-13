#pragma once

#include "Camera.h"

namespace IL
{

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjectionMatrix(float left, float right, float bottom, float top);

		virtual const glm::mat4& GetViewMatrix()	       const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetProjectionMatrix()	   const override { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectMatrix; }

		virtual const glm::vec3& GetPosition() const override { return m_Position; }
		virtual void SetPosition(const glm::vec3& position) override { m_Position = position; ReCalViewMatrix(); }

		virtual const float& GetRotation() const override { return m_Rotation; }
		virtual void SetRotation(const float& rotation) override { m_Rotation = rotation; ReCalViewMatrix(); }
	private:
		virtual void ReCalViewMatrix() override;
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectMatrix;

		float     m_Rotation;
		glm::vec3 m_Position;
	};

}