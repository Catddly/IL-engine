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
	static bool p_open = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		// Always be the top of the view
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigDockingWithShift = false;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	//else
	//{
	//	ShowDockingDisabledMessage();
	//}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Docking"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
				dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
				dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				Application::GetApplication().Close();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();

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
