#include <IL.h>

#include "Platform/OpenGL/OpenGlShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <future>
#include <thread>
#include <mutex>

using namespace IL;

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraPosition({0.0f, 0.0f, 0.0f}), m_CameraRotation(0.0f)
	{
		m_Camera = Camera::CreateOrtho(-1.6f, 1.6f, -0.9f, 0.9f);

		// Initialize draw call data of OpenGL
		// VertexArray
		m_VertexArray.reset(VertexArray::Create());

		// one square
		float vertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// IndexBuffer
		uint32_t indices[6] = { 0, 1, 2, 0, 3, 1 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset((IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// create shader
		std::string vertexShaderTex = R"(
            #version 460 core

			layout(location = 0) in vec3 a_Position;		
			layout(location = 1) in vec2 a_TexCoord;		

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}	
		)";

		std::string fragmentShaderTex = R"(
            #version 460 core

			layout(location = 0) out vec4 color;
	
			uniform sampler2D u_Texture;

			in vec2 v_TexCoord;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}	
		)";

		m_ShaderTex.reset(Shader::Create(vertexShaderTex, fragmentShaderTex));

		m_Texture = Texture2D::Create("assets/textures/ILLmew.png");
		std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTex)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTex)->UploadUniformInt("u_Texture", 0 /* slot id */);

		/// <summary>
		/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// </summary>

		std::string vertexShader = R"(
            #version 460 core

			layout(location = 0) in vec3 a_Position;		
			layout(location = 1) in vec2 a_TexCoord;		

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}	
		)";

		std::string fragmentShader = R"(
            #version 460 core

			layout(location = 0) out vec4 color;
	
			uniform vec3 u_SquareColor;

			in vec2 v_TexCoord;

			void main()
			{
				color = vec4(u_SquareColor, 1.0f);
			}	
		)";

		m_Shader.reset(Shader::Create(vertexShader, fragmentShader));
		m_Shader->Bind();
	}

	void OnUpdate(TimeStep dt) override
	{
		//IL_TRACE("dt: {0}, {1}", dt.GetSeconds(), dt.GetMilliseconds());

		if (Input::IsKeyPressed(IL_KEY_A))
			m_CameraPosition.x -= camMoveSpeed * dt;
		else if (Input::IsKeyPressed(IL_KEY_D))
			m_CameraPosition.x += camMoveSpeed * dt;

		if (Input::IsKeyPressed(IL_KEY_W))
			m_CameraPosition.y += camMoveSpeed * dt;
		else if (Input::IsKeyPressed(IL_KEY_S))
			m_CameraPosition.y -= camMoveSpeed * dt;

		if (Input::IsKeyPressed(IL_KEY_Q))
			m_CameraRotation -= camRotationSpeed * dt;
		else if (Input::IsKeyPressed(IL_KEY_E))
			m_CameraRotation += camRotationSpeed * dt;

		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera);

		m_Camera->SetPosition(m_CameraPosition);
		m_Camera->SetRotation(m_CameraRotation);

		glm::mat4 transformTex = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int x = -10; x < 10; x++)
		{
			for (int y = -10; y < 10; y++)
			{
				glm::vec3 pos = glm::vec3(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Renderer::Submit(m_Shader, m_VertexArray, transform);
				std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->UploadUniformFloat3("u_SquareColor", m_SquareColor);
			}
		}
		
		m_Texture->Bind();
		Renderer::Submit(m_ShaderTex, m_VertexArray, transformTex);

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(IL::Event& e) override 
	{
	}

private:
	Ref<Shader> m_ShaderTex, m_Shader;
	Ref<VertexArray> m_VertexArray; // it contains the vertexBuffers and indexBuffer
	Ref<Camera> m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraRotation;

	Ref<Texture2D> m_Texture;

	float camMoveSpeed = 2.0f;
	float camRotationSpeed = 100.0f;

	glm::vec3 m_SquareColor = glm::vec3(0.4f);
};

class TestLayer : public Layer
{
public:
	TestLayer() : Layer("TimerLayer")
	{
		Timer timer("TestLayer::TestLayer()");

		//Count();
		//std::async(std::launch::async, Count);

		std::thread t1(Count);
		std::thread t2(Count);
		std::thread t3(Count);

		t1.join();
		t2.join();
		t3.join();

		IL_CORE_TRACE("TestLayer print: {0}", m_Counter);
	}

	static void Count()
	{
		mutex.lock();
		for (int i = 0; i < 10000000; i++)
		{
			m_Counter += 2;
		}
		mutex.unlock();
	}

private:
	static long long int m_Counter;
	static std::mutex mutex;
};

long long int TestLayer::m_Counter = 0;
std::mutex TestLayer::mutex;

class SandBox : public IL::Application
{
public: 
	SandBox()
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new TestLayer());
	}

	~SandBox()
	{
	}

};

IL::Application* IL::CreateApplication()
{
	return new SandBox;
}
