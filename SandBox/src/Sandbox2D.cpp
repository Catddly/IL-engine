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

	m_Texture1 = Texture2D::Create("assets/textures/CheckerBoard.png");
	m_Texture2 = Texture2D::Create("assets/textures/ILLmew.png");
	m_TileMap = Texture2D::Create("assets/textures/mapPack_tilesheet_2X.png");

	m_TreeTex = SubTexture2D::CreateFromCoord(m_TileMap, { 3, 4 }, { 128, 128 });
	m_SnowmanTex = SubTexture2D::CreateFromCoord(m_TileMap, { 3, 5 }, { 128, 128 });
	m_FloorTex = SubTexture2D::CreateFromCoord(m_TileMap, { 0, 5 }, { 128, 128 }, { 3, 3 });
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

	Renderer2D::ResetStats();
	{
		IL_PROFILE_SCOPE("Sandbox2DLayer::Prep");
		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
		RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += dt * 50.0f;

		IL_PROFILE_SCOPE("Sandbox2DLayer::RenderDrawCall");

		Renderer2D::BeginScene(m_CameraController->GetCamera());

		//Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.99f }, { 6.0f, 6.0f }, m_Texture2, m_TilingFactor);
		Renderer2D::DrawQuad({ 0.0f, 0.0f, m_Depth }, { 5.0f, 5.0f }, m_Texture1, m_TilingFactor);
		Renderer2D::DrawQuad({ 0.0f, 1.8f, 0.3f }, { 1.0f, 1.0f }, m_TreeTex);
		Renderer2D::DrawQuad({ -0.6f, 1.8f, 0.31f }, { 1.0f, 1.0f }, m_TreeTex);
		Renderer2D::DrawQuad({ 0.6f, 1.8f, 0.32f }, { 1.0f, 1.0f }, m_SnowmanTex);
		Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.33f }, { 1.0f, 1.0f }, m_FloorTex);
		//Renderer2D::DrawRotatedQuad({ 2.0f, 2.0f, 0.0f }, { 2.0f, 1.25f }, rotation, m_SquareColor);

		//for (float x = -0.5f; x <= 0.5f; x += 0.05f)
		//{
		//	for (float y = -0.5f; y <= 0.5f; y += 0.05f)
		//	{
		//		glm::vec4 color = { 1.0f, 0.5 + x, 0.5 + y, m_Alpha };
		//		Renderer2D::DrawQuad({ x * 5.0f, y * 5.0f, 0.1f }, { 0.2f, 0.2f }, color);
		//	}
		//}

		Renderer::EndScene();
	}
}

void Sandbox2DLayer::OnImGuiRender()
{
#if IL_DEBUG
	auto stats = Renderer2D::GetStats();

	ImGui::Begin("Statistics");
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
#endif

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor), ImGuiColorEditFlags_DisplayHSV);
	ImGui::SliderFloat("Depth", &m_Depth, -0.999f, 1.0f);
	ImGui::SliderFloat("TilingFactor", &m_TilingFactor, -10.0f, 10.0f);
	ImGui::SliderFloat("alpha", &m_Alpha, 0.0f, 1.0f);
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_CameraController->OnEvent(e);
}
