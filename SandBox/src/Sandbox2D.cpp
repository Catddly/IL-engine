#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGlShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2DLayer::Sandbox2DLayer()
	:Layer("Sandbox2D"), m_CameraController(std::make_shared<OrthographicCameraController>(1280.0f / 720.0f, true))
{
}

void Sandbox2DLayer::OnAttach()
{
	m_VertexArray = VertexArray::Create();
	// one square
	float vertices[4 * 3] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f
	};
	Ref<VertexBuffer> vertexBuffer;
	vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
	BufferLayout layout =
	{
		{ ShaderDataType::Float3, "a_Position" }
	};
	vertexBuffer->SetLayout(layout);

	// Initialize draw call data of OpenGL
	// VertexArray
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	// IndexBuffer
	uint32_t indices[6] = { 0, 1, 2, 0, 3, 1 };
	Ref<IndexBuffer> indexBuffer;
	indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_ShaderLibrary.Load("assets/shaders/Square.glsl");
	m_SquareShader = m_ShaderLibrary.Get("Square");
}

void Sandbox2DLayer::OnDeatch()
{
}

void Sandbox2DLayer::OnUpdate(TimeStep dt)
{
	m_CameraController->OnUpdate(dt);

	RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController->GetCamera());

	m_SquareShader->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(m_SquareShader)->UploadUniformFloat4("u_SquareColor", m_SquareColor);

	Renderer::Submit(m_SquareShader, m_VertexArray);

	Renderer::EndScene();
}

void Sandbox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_CameraController->OnEvent(e);
}
