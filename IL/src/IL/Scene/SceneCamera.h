#pragma once

#include "IL/Renderer/Camera/Camera.h"

namespace IL
{

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Pespective = 0, Orthograghic = 1 };

		SceneCamera() = default;
		~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPespective(float fov /* radians */, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPespectiveYFOV() const { return m_PespectiveVerticleFOV; }
		void SetPespectiveYFOV(float fov) { m_PespectiveVerticleFOV = fov; RecalculateProjectionMatrix(); }
		float GetPespectiveNearClip() const { return m_PespectiveNear; }
		void SetPespectiveNearClip(float nearClip) { m_PespectiveNear = nearClip; RecalculateProjectionMatrix(); }
		float GetPespectiveFarClip() const { return m_PespectiveFar; }
		void SetPespectiveFarClip(float farClip) { m_PespectiveFar = farClip; RecalculateProjectionMatrix(); }

		float GetOrthographicSize() const { return m_OrthoSize; }
		void SetOrthographicSize(float size) { m_OrthoSize = size; RecalculateProjectionMatrix(); }
		float GetOrthographicNearClip() const { return m_OrthoNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthoNear = nearClip; RecalculateProjectionMatrix(); }
		float GetOrthographicFarClip() const { return m_OrthoFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthoFar = farClip; RecalculateProjectionMatrix(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjectionMatrix(); }
	private:
		void RecalculateProjectionMatrix();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthograghic;

		float m_PespectiveVerticleFOV = glm::radians(45.0f);
		float m_PespectiveNear = 1.0f, m_PespectiveFar = 10000.0f;

		float m_OrthoSize = 5.0f;
		float m_OrthoNear = -20.0f, m_OrthoFar = 20.0f;

		float m_AspectRatio = 0.0f;
	};

}