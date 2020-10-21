#include "ilpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IL
{

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthoSize = size;
		m_OrthoNear = nearClip;
		m_OrthoFar = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetPespective(float fov, float nearClip, float farClip)
	{
		m_PespectiveVerticleFOV = fov;
		m_PespectiveNear = nearClip;
		m_PespectiveFar = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{
		if (m_ProjectionType == ProjectionType::Pespective)
		{
			m_ProjectionMatrix = glm::perspective(m_PespectiveVerticleFOV, m_AspectRatio, 
				m_PespectiveNear, m_PespectiveFar);
		}
		else
		{
			float orthoLeft = -0.5f * m_OrthoSize * m_AspectRatio;
			float orthoRight = 0.5f * m_OrthoSize * m_AspectRatio;
			float orthoBottom = -m_OrthoSize * 0.5f;
			float orthoTop = m_OrthoSize * 0.5f;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthoNear, m_OrthoFar);
		}
	}

}