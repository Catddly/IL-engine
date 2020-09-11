#include "Sandbox2D.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2DLayer::Sandbox2DLayer()
	:Layer("Sandbox2D"), m_CameraController(std::make_shared<OrthographicCameraController>(1280.0f / 720.0f, true))
{
}

void Sandbox2DLayer::OnAttach()
{
	m_Texture = Texture2D::Create("assets/textures/CheckerBoard.png");
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

	Renderer2D::DrawQuad({ 0.0f, 0.0f }, m_SquareRotation, { 2.0f, 1.25f }, m_SquareColor);
	Renderer2D::DrawQuad({ 1.0f, 1.5f }, 0.0f, { 0.75f, 0.5f }, { 0.4f, 0.9f, 0.4f, 1.0f });
	Renderer2D::DrawQuad({ 0.0f, 0.0f, m_Depth }, 0.0f, { 5.0f, 5.0f }, m_Texture, m_UVScaling);

	Renderer::EndScene();
}

void Sandbox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor), ImGuiColorEditFlags_DisplayHSV);
	ImGui::SliderFloat("Rotation", &m_SquareRotation, 0.0f, 360.0f);
	ImGui::SliderFloat("Depth", &m_Depth, -0.999f, 1.0f);
	ImGui::SliderFloat("UV", &m_UVScaling, -10.0, 10.0f);
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_CameraController->OnEvent(e);
}
