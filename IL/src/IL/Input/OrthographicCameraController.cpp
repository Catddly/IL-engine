#include "ilpch.h"
#include "OrthographicCameraController.h"

#include "IL/Input/Input.h"
#include "IL/Input/KeyCodes.h"

namespace IL
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool isRotate)
		:m_Camera(std::make_shared<OrthographicCamera>(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)),
		m_IsRotate(isRotate), m_AspectRatio(aspectRatio)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep dt)
	{
		//IL_TRACE("dt: {0}, {1}", dt.GetSeconds(), dt.GetMilliseconds());

		if (Input::IsKeyPressed(IL_KEY_A))
			m_Position.x -= m_CameraMoveSpeed * dt;
		else if (Input::IsKeyPressed(IL_KEY_D))
			m_Position.x += m_CameraMoveSpeed * dt;

		if (Input::IsKeyPressed(IL_KEY_W))
			m_Position.y += m_CameraMoveSpeed * dt;
		else if (Input::IsKeyPressed(IL_KEY_S))
			m_Position.y -= m_CameraMoveSpeed * dt;

		if (m_IsRotate)
		{
			if (Input::IsKeyPressed(IL_KEY_Q))
				m_Rotation -= m_CamRotationSpeed * dt;
			else if (Input::IsKeyPressed(IL_KEY_E))
				m_Rotation += m_CamRotationSpeed * dt;

			m_Camera->SetRotation(m_Rotation);
		}

		m_Camera->SetPosition(m_Position);
		m_CameraMoveSpeed = m_ZoomLevel * m_CameraMoveRatio;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(IL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
		dispatcher.Dispatch<MouseScrolledEvent>(IL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		//dispatcher.Dispatch<MouseButtonPressedEvent>(IL_BIND_EVENT_FN(OrthographicCameraController::OnMousePressed));
		//dispatcher.Dispatch<MouseButtonReleasedEvent>(IL_BIND_EVENT_FN(OrthographicCameraController::OnMouseReleased));
		//dispatcher.Dispatch<MouseMovedEvent>(IL_BIND_EVENT_FN(OrthographicCameraController::OnMouseMoved));
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera->SetProjectionMat(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() / 10.0f;

		m_ZoomLevel = std::max(m_ZoomLevel, 0.3f);
		m_ZoomLevel = std::min(m_ZoomLevel, 3.0f);

;		m_Camera->SetProjectionMat(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnMousePressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool OrthographicCameraController::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		return false;
	}

	bool OrthographicCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		return false;
	}

}