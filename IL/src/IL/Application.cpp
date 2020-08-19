#include "ilpch.h"

#include "Application.h"

#include <glad/glad.h>

#include "IL/Input.h"

namespace IL						
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		IL_CORE_ASSERT(!s_Instance, "Already had one application!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(IL_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		// Initialize draw call data of OpenGL
		// VertexArray
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// VertexBuffer
		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.0f,  0.85f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};
		// send the vertices data from CPU to GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		// explain the data to the GPU
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// IndexBuffer
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// create shader
		std::string vertexShader = R"(
            #version 460 core

			layout(location = 0) in vec3 a_Position;			

			out vec3 v_Position;			

			void main()
			{
				v_Position = a_Position * 2.0;
				gl_Position = vec4(a_Position * 2.0, 1.0);
			}	
		)";

		std::string fragmentShader = R"(
            #version 460 core

			layout(location = 0) out vec4 color;			

			in vec3 v_Position;			

			void main()
			{
				color = vec4((v_Position + 1.0) * 0.5, 1.0);
			}	
		)";

		m_Shader.reset(new Shader(vertexShader, fragmentShader));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// draw call on every frame
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// render ImGui
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			// native window update
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
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
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{	
		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == 256 /* esc */)
		{
			WindowCloseEvent e;
			OnWindowClose(e);
		}
		else if (e.GetKeyCode() == 84)
		{
			m_ShowWindowTrace = !m_ShowWindowTrace;
		}
		return true;
	}

}