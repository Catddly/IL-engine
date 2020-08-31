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


		m_ShaderTex.reset(Shader::Create("assets/shaders/Texture.glsl"));
		m_Texture = Texture2D::Create("assets/textures/ILLmew.png");
		std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTex)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTex)->UploadUniformInt("u_Texture", 0 /* slot id */);

		m_Shader.reset(Shader::Create("assets/shaders/Square.glsl"));
		m_Shader->Bind();

		m_ShaderTexAlpha.reset(Shader::Create("assets/shaders/TextureAlpha.glsl"));
		m_Texture1 = Texture2D::Create("assets/textures/design.png");
		std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTexAlpha)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTexAlpha)->UploadUniformInt("u_Texture", 0/* slot id */);
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
		glm::mat4 slightTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.0f));

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
		m_Texture1->Bind();
		Renderer::Submit(m_ShaderTexAlpha, m_VertexArray, transformTex);

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
	Ref<Shader> m_ShaderTex, m_Shader, m_ShaderTexAlpha;
	Ref<VertexArray> m_VertexArray; // it contains the vertexBuffers and indexBuffer
	Ref<Camera> m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraRotation;

	Ref<Texture2D> m_Texture, m_Texture1;

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
