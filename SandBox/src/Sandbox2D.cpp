#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGlShader.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2DLayer::Sandbox2DLayer()
	:Layer("Sandbox2D"), m_CameraController(std::make_shared<OrthographicCameraController>(1280.0f / 720.0f, true))
{
}

void Sandbox2DLayer::OnAttach()
{
}

void Sandbox2DLayer::OnDeatch()
{
}

void Sandbox2DLayer::OnUpdate(TimeStep dt)
{
	m_CameraController->OnUpdate(dt);

	RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController->GetCamera());

	Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 2.0f, 1.25f }, m_SquareColor);

	Renderer::EndScene();
}

void Sandbox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_CameraController->OnEvent(e);
}
