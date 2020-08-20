#include "ilpch.h"

#include "Application.h"

#include <glad/glad.h>
#include "IL/Input.h"

#include "IL/Renderer/Renderer.h"
#include "IL/Renderer/Buffer.h"

namespace IL						
{

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataType2GLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case IL::ShaderDataType::Float:  
		case IL::ShaderDataType::Float2:			
		case IL::ShaderDataType::Float3:		
		case IL::ShaderDataType::Float4: 
		case IL::ShaderDataType::Mat3:			
		case IL::ShaderDataType::Mat4:   return GL_FLOAT;
		case IL::ShaderDataType::Int:			
		case IL::ShaderDataType::Int2:			
		case IL::ShaderDataType::Int3:			
		case IL::ShaderDataType::Int4:	 return GL_INT;
		case IL::ShaderDataType::Bool:   return GL_BOOL;
		}

		IL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application()
	{
		IL_CORE_ASSERT(!s_Instance, "Already had one application!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(IL_BIND_EVENT_FN(Application::OnEvent));

		// initialize ImGui
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		// Initialize draw call data of OpenGL
		// VertexArray
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// VertexBuffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f,  0.0f, 0.8f, 0.7f, 0.0f, 1.0f,
			 0.0f,  0.85f, 0.0f, 0.1f, 0.8f, 0.8f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.7f, 0.0f, 0.9f, 1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// create and send to VertexBuffer, then destroy
		{
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};
			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			// explain the data to the GPU
			glVertexAttribPointer(index, 
				element.GetComponentCount(),
				ShaderDataType2GLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStrike(),
				(const void*)element.Offset);
			index++;
		}

		// IndexBuffer
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset((IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))));

		// create shader
		std::string vertexShader = R"(
            #version 460 core

			layout(location = 0) in vec3 a_Position;		
			layout(location = 1) in vec4 a_Color;	

			out vec3 v_Position;	
			out vec4 v_Color;		

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position * 1.1;
				gl_Position = vec4(a_Position * 1.1, 1.0);
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
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