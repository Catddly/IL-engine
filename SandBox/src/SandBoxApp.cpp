#include <IL.h>

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

#include <future>
#include <thread>
#include <mutex>

// -------------EntryPoint---------------
#include "IL/Core/EntryPoint.h"
// --------------------------------------

using namespace IL;

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(std::make_shared<OrthographicCameraController>(1280.0f / 720.0f, true))
	{
		// Initialize draw call data of OpenGL
		// VertexArray
		m_VertexArray = VertexArray::Create();

		// one square
		float vertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
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
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_Texture = Texture2D::Create("assets/textures/ILLmew.png");
		m_ShaderLibrary.Get("Texture")->Bind();
		m_ShaderLibrary.Get("Texture")->SetInt("u_Texture", 0 /* slot id */);

		m_ShaderLibrary.Load("assets/shaders/Square.glsl");
		m_ShaderLibrary.Get("Square")->Bind();

		m_ShaderLibrary.Load("assets/shaders/TextureAlpha.glsl");
		m_Texture1 = Texture2D::Create("assets/textures/design.png");
		m_ShaderLibrary.Get("TextureAlpha")->Bind();
		m_ShaderLibrary.Get("TextureAlpha")->SetInt("u_Texture", 0/* slot id */);
	}

	void OnUpdate(TimeStep dt) override
	{
		m_CameraController->OnUpdate(dt);

		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
		RenderCommand::Clear();

		Renderer::BeginScene(m_CameraController->GetCamera());

		glm::mat4 transformTex = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::mat4 slightTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.0f));

		for (int x = -10; x < 10; x++)
		{
			for (int y = -10; y < 10; y++)
			{
				glm::vec3 pos = glm::vec3(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Renderer::Submit(m_ShaderLibrary.Get("Square"), m_VertexArray, transform);
				m_ShaderLibrary.Get("Square")->SetFloat3("u_SquareColor", m_SquareColor);
			}
		}
		
		m_Texture->Bind();
		Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_VertexArray, transformTex);
		m_Texture1->Bind();
		Renderer::Submit(m_ShaderLibrary.Get("TextureAlpha"), m_VertexArray, transformTex);

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Event& e) override 
	{
		m_CameraController->OnEvent(e);
	}

private:
	ShaderLibrary m_ShaderLibrary;

	Ref<VertexArray> m_VertexArray; // it contains the vertexBuffers and indexBuffer
	Ref<OrthographicCameraController> m_CameraController;

	Ref<Texture2D> m_Texture, m_Texture1;

	glm::vec3 m_SquareColor = glm::vec3(0.4f);
};

//class TestLayer : public Layer
//{
//public:
//	TestLayer() : Layer("TimerLayer")
//	{
//		Timer timer("TestLayer::TestLayer()");
//
//		//Count();
//		//std::async(std::launch::async, Count);
//
//		std::thread t1(Count);
//		std::thread t2(Count);
//		std::thread t3(Count);
//
//		t1.join();
//		t2.join();
//		t3.join();
//
//		IL_CORE_TRACE("TestLayer print: {0}", m_Counter);
//	}
//
//	static void Count()
//	{
//		mutex.lock();
//		for (int i = 0; i < 10000000; i++)
//		{
//			m_Counter += 2;
//		}
//		mutex.unlock();
//	}
//
//private:
//	static long long int m_Counter;
//	static std::mutex mutex;
//};
//
//long long int TestLayer::m_Counter = 0;
//std::mutex TestLayer::mutex;

class SandBox : public IL::Application
{
public: 
	SandBox()
		: Application("SandBox")
	{
		//PushLayer(new ExampleLayer());
		//PushOverlay(new TestLayer());
		PushLayer(new Sandbox2DLayer());
	}
	~SandBox()
	{
	}
};

IL::Application* IL::CreateApplication()
{
	return new SandBox;
}
