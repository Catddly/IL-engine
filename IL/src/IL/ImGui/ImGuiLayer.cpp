#include "ilpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "IL/Core/Application.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "IL/Events/MouseEvent.h"

namespace IL
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IL_PROFILE_FUNCTION();

		// Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Set Fonts
		io.Fonts->AddFontFromFileTTF("assets/fonts/Source_Sans_Pro/SourceSansPro-Bold.ttf", 20.f);
		ImFont* myRegularFont = io.Fonts->AddFontFromFileTTF("assets/fonts/Source_Sans_Pro/SourceSansPro-Regular.ttf", 20.0f);
		io.FontDefault = myRegularFont;

		// Setup Dear ImGui style
		ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDeatch()
	{
		IL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(IL_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolled));
		dispatcher.Dispatch<KeyPressedEvent>(IL_BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));
	}

	void ImGuiLayer::OnImGuiRender()
	{
	}

	void ImGuiLayer::Begin()
	{
		IL_PROFILE_FUNCTION();

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		IL_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetApplication();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.973f, 0.976f, 0.980f, 1.0f };

		// Headers
		// ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_Header] = ImVec4{ 0.992f, 0.886f, 0.894f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.980f, 0.824f, 0.882f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 1.0f, 0.686f, 0.8f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.796f, 0.952f, 0.941f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.745f, 0.953f, 0.941f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.745f, 0.953f, 0.941f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.973f, 0.986f, 0.992f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.804f, 0.855f, 0.992f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.804f, 0.855f, 0.992f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.85f, 0.98f, 1.0f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.118f, 0.588f, 0.988f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.15f, 0.9f, 0.91f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.905f, 0.951f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.804f, 0.855f, 0.992f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.9f, 1.0f, 1.0f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.79f, 1.0f, 1.0f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.9f, 0.1505f, 0.9f, 1.0f };
	}

	bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

}