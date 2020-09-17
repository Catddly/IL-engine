#include "Sandbox2D.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2DLayer::Sandbox2DLayer()
	:Layer("Sandbox2D"), m_CameraController(CreateRef<OrthographicCameraController>(1280.0f / 720.0f, true))
{
}

void Sandbox2DLayer::OnAttach()
{
	IL_PROFILE_FUNCTION();

	m_Texture = Texture2D::Create("assets/textures/CheckerBoard.png");
}

void Sandbox2DLayer::OnDeatch()
{
	IL_PROFILE_FUNCTION();
}

void Sandbox2DLayer::OnUpdate(TimeStep dt)
{
	IL_PROFILE_FUNCTION();
	{
		IL_PROFILE_SCOPE("Sandbox2DLayer::CameraUpdate");
		m_CameraController->OnUpdate(dt);
	}

	{
		IL_PROFILE_SCOPE("Sandbox2DLayer::Prep");
		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
		RenderCommand::Clear();
	}

	{
		IL_PROFILE_SCOPE("Sandbox2DLayer::Renderer");
		Renderer2D::BeginScene(m_CameraController->GetCamera(), true);

		//Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, m_SquareRotation, { 2.0f, 1.25f }, m_SquareColor);
		Renderer2D::DrawQuad({ 0.3f, -0.4f }, { 0.75f, 0.5f }, { 0.4f, 0.9f, 0.4f, 1.0f });
		Renderer2D::DrawQuad({ 0.3f, 0.4f }, { 1.0f, 0.7f }, m_SquareColor);
		//Renderer2D::DrawQuad({ 0.0f, 0.0f, m_Depth }, { 5.0f, 5.0f }, m_Texture, m_UVScaling);

		Renderer::EndScene();
	}
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
