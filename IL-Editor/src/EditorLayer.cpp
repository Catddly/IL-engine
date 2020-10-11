#include "EditorLayer.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

namespace IL
{

	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(CreateRef<OrthographicCameraController>(1280.0f / 720.0f, true))
	{
	}

	void EditorLayer::OnAttach()
	{
		IL_PROFILE_FUNCTION();

		m_Texture1 = Texture2D::Create("assets/textures/CheckerBoard.png");
		m_Texture2 = Texture2D::Create("assets/textures/ILLmew.png");
		m_TileMap = Texture2D::Create("assets/textures/mapPack_tilesheet_2X.png");

		m_TreeTex = SubTexture2D::CreateFromCoord(m_TileMap, { 3, 4 }, { 128, 128 });
		m_SnowmanTex = SubTexture2D::CreateFromCoord(m_TileMap, { 3, 5 }, { 128, 128 });
		m_FloorTex = SubTexture2D::CreateFromCoord(m_TileMap, { 0, 5 }, { 128, 128 }, { 3, 3 });

		FrameBufferProps myProps;
		myProps.Width = 1280;
		myProps.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(myProps);

		m_ActiveScene = CreateRef<Scene>();
		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_ScriptEntity = m_ActiveScene->CreateEntity("Scripted square");

		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.9f, 0.2f, 1.0f });
		m_ScriptEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

		auto& transform = m_ScriptEntity.GetComponent<TransformComponent>();
		transform.Transform *= glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));

		class SquareAnimated : public SciptableEntity
		{
		public:
			void OnCreate()
			{
				IL_CORE_INFO("SquareAnimated::OnCreate");
			}

			void OnUpdate(TimeStep dt)
			{
				auto& transform = GetComponent<TransformComponent>();
				static float clock = 0.0f;
				float speed = 3.0f;
				float amplitude = 0.7f;

				transform.Transform[3][0] = glm::sin(clock) * amplitude;
				transform.Transform[3][1] = glm::cos(clock) * amplitude;
				clock += dt * speed;
			}
		};

		m_ScriptEntity.AddComponent<NativeScriptComponent>().Bind<SquareAnimated>();
	}

	void EditorLayer::OnDeatch()
	{
		IL_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(TimeStep dt)
	{
		IL_PROFILE_FUNCTION();
		{
			IL_PROFILE_SCOPE("EditorLayer::CameraUpdate");
			if (m_ViewportFocused)
				m_CameraController->OnUpdate(dt);
		}

		Renderer2D::ResetStats();
		{
			IL_PROFILE_SCOPE("EditorLayer::Prep");
			m_FrameBuffer->Bind();
			RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
			RenderCommand::Clear();
		}

		{
			IL_PROFILE_SCOPE("EditorLayer::RenderDrawCall");

			Renderer2D::BeginScene(m_CameraController->GetCamera());

			m_ActiveScene->OnUpdate(dt);

			//Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.99f }, { 6.0f, 6.0f }, m_Texture2, m_TilingFactor);
			//Renderer2D::DrawQuad({ 0.0f, 0.0f, m_Depth }, { 5.0f, 5.0f }, m_Texture1, m_TilingFactor);
			//Renderer2D::DrawQuad({ 0.0f, 1.8f, 0.3f }, { 1.0f, 1.0f }, m_TreeTex);
			//Renderer2D::DrawQuad({ -0.6f, 1.8f, 0.31f }, { 1.0f, 1.0f }, m_TreeTex);
			//Renderer2D::DrawQuad({ 0.6f, 1.8f, 0.32f }, { 1.0f, 1.0f }, m_SnowmanTex);
			//Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.33f }, { 1.0f, 1.0f }, m_FloorTex);
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
			m_FrameBuffer->UnBind();
		}
	}

	void EditorLayer::OnImGuiRender()
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
		// hold shift to dock
		io.ConfigDockingWithShift = true;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File and settings"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				//if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
				//	dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				//if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
				//	dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
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
		uint32_t texture = m_Texture2->GetRendererID();
		ImGui::Image((void*)texture, ImVec2{ 256.0f, 256.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
#endif
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		// Draw frameBuffer on viewport
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		uint32_t colorAttachTexture = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)colorAttachTexture, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		// Resize viewport
		if (viewportSize.x != m_ViewportSize.x || viewportSize.y != m_ViewportSize.y)
		{
			m_FrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_CameraController->OnResize(viewportSize.x, viewportSize.y);
		}
		ImGui::End();

		ImGui::PopStyleVar();

		ImGui::Begin("Settings");

		ImGui::SliderFloat("Depth", &m_Depth, -0.999f, 1.0f);
		ImGui::SliderFloat("TilingFactor", &m_TilingFactor, -10.0f, 10.0f);
		ImGui::SliderFloat("alpha", &m_Alpha, 0.0f, 1.0f);

		if (m_SquareEntity)
		{
			ImGui::Separator();
			auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());

			auto& spriteCom = m_SquareEntity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Square color", glm::value_ptr(spriteCom.Color), ImGuiColorEditFlags_DisplayHSV);
			ImGui::Separator();
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (m_ViewportFocused && m_ViewportHovered)
			m_CameraController->OnEvent(e);
	}

}