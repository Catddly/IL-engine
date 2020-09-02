#pragma once

#include "IL/Renderer/Camera/OrthographicCamera.h"
#include "IL/Core/TimeStep.h"

#include "IL/Events/MouseEvent.h"
#include "IL/Events/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace IL
{

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool isRotate = true);

		const Ref<OrthographicCamera>& GetCamera() const { return m_Camera; }

		void OnUpdate(TimeStep dt);
		void OnEvent(Event& e);
	private:
		bool OnWindowResize(WindowResizeEvent& e);

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		Ref<OrthographicCamera> m_Camera;

		bool m_IsRotate;
		float m_Rotation = 0.0f;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_CameraMoveSpeed;
		float m_CameraMoveRatio = 2.0f;
		float m_CamRotationSpeed = 100.0f;
	};

}