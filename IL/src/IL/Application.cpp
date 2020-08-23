#include "ilpch.h"

#include "Application.h"

#include "IL/Input.h"

#include "IL/Renderer/Renderer.h"
#include "IL/Renderer/Camera/Camera.h"

namespace IL						
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		IL_CORE_ASSERT(!s_Instance, "Already had one application!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(IL_BIND_EVENT_FN(Application::OnEvent));

		// initialize ImGui
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		m_Camera = Camera::CreateOrtho(-1.6f, 1.6f, -0.9f, 0.9f);

		// Initialize draw call data of OpenGL
		// VertexArray
		m_VertexArray.reset(VertexArray::Create());

		// VertexBuffer
		//float vertices[3 * 7] = {
		//	-0.5f, -0.5f,  0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
		//	 0.0f,  0.5f, 0.0f, 0.1f, 0.8f, 0.8f, 1.0f,
		//	 0.5f, -0.5f,  0.0f, 0.7f, 0.0f, 0.9f, 1.0f
		//};

		float vertices[4 * 7] = {
			-0.5f, -0.5f,  0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.0f, 0.1f, 0.8f, 0.8f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.7f, 0.0f, 0.9f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.2f, 0.5f, 0.9f, 1.0f,
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// IndexBuffer
		uint32_t indices[6] = { 0, 1, 2, 0, 3, 1 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset((IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// create shader
		std::string vertexShader = R"(
            #version 460 core

			layout(location = 0) in vec3 a_Position;		
			layout(location = 1) in vec4 a_Color;	

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;	
			out vec4 v_Color;		

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position * 1.1;
				gl_Position = u_ViewProjection * vec4(a_Position * 1.1, 1.0);
			}	
		)";

		std::string fragmentShader = R"(
            #version 460 core

			layout(location = 0) out vec4 color;			

			in vec3 v_Position;		
			in vec4 v_Color;			

			void main()
			{
				color = vec4((v_Position + 1.0) * 0.5, 1.0);
				color = v_Color;
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
			RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

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
		RenderCommand::SetViewPortSize(e.GetWidth(), e.GetHeight());
		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == 256 /* esc */)
		{
			WindowCloseEvent e;
			OnWindowClose(e);
		}
		else if (e.GetKeyCode() == 84 /* t */)
		{
			m_ShowWindowTrace = !m_ShowWindowTrace;
		}
		else if (e.GetKeyCode() == 87 /* w */)
		{
			glm::vec3 pos = m_Camera->GetPosition();
			pos.y += 0.03f;
			m_Camera->SetPosition(pos);
		}
		else if (e.GetKeyCode() == 65 /* a */)
		{
			glm::vec3 pos = m_Camera->GetPosition();
			pos.x -= 0.03f;
			m_Camera->SetPosition(pos);
		}
		else if (e.GetKeyCode() == 83 /* s */)
		{
			glm::vec3 pos = m_Camera->GetPosition();
			pos.y -= 0.03f;
			m_Camera->SetPosition(pos);
		}
		else if (e.GetKeyCode() == 68 /* d */)
		{
			glm::vec3 pos = m_Camera->GetPosition();
			pos.x += 0.03f;
			m_Camera->SetPosition(pos);
		}
		else if (e.GetKeyCode() == 81 /* q */)
		{
			float rotation = m_Camera->GetRotation();
			rotation -= 5.0f;
			m_Camera->SetRotation(rotation);
		}
		else if (e.GetKeyCode() == 69 /* e */)
		{
			float rotation = m_Camera->GetRotation();
			rotation += 5.0f;
			m_Camera->SetRotation(rotation);
		}

		return true;
	}

}