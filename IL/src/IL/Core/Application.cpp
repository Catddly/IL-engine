#include "ilpch.h"

#include "Application.h"

#include "IL/Input/Input.h"
#include "IL/Renderer/Renderer.h"
#include "IL/Renderer/RenderCommand.h"

#include "IL/Renderer/GraphicContext.h"

#include <GLFW/glfw3.h>

namespace IL						
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
		:m_LastFrameTime(0.0f)
	{
		IL_PROFILE_FUNCTION();

		IL_CORE_ASSERT(!s_Instance, "Already had one application!");
		s_Instance = this;

		WindowProps windowProps(name, 1920, 1080);
		m_Window = Window::Create(windowProps);
		m_Window->SetEventCallback(IL_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		// initialize ImGui
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		IL_PROFILE_FUNCTION();

		for (Layer* layer : m_LayerStack)
		{
			layer->OnDeatch();
		}

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		IL_PROFILE_FUNCTION();

		while (m_Running)
		{
			IL_PROFILE_SCOPE("Application RunLoop");

			float time = (float)glfwGetTime();
			TimeStep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					IL_PROFILE_SCOPE("LayerStack OnUpdate()");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				// render ImGui
				m_ImGuiLayer->Begin();
				{
					IL_PROFILE_SCOPE("LayerStack OnImGuiRender()");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			{
				IL_PROFILE_SCOPE("Window OnUpdate()");

				// native window update
				m_Window->OnUpdate();
			}
		}
	}

	void Application::OnEvent(Event& e)
	{
		IL_PROFILE_FUNCTION();

		// dispatch the event to Window class, check and execute it
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(IL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<KeyPressedEvent>(IL_BIND_EVENT_FN(Application::OnKeyPressed));
		dispatcher.Dispatch<WindowResizeEvent>(IL_BIND_EVENT_FN(Application::OnWindowResize));

		if (m_ShowWindowTrace)
		{
			IL_CORE_TRACE("{0}", e);
		}

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			// if some layer from the end of the layers receive the event, stop
			if (e.IsHandled())
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		IL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		IL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{	
		IL_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		m_Minimized = false;
		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == 84 /* t */)
		{
			m_ShowWindowTrace = !m_ShowWindowTrace;
		}
		return true;
	}

}