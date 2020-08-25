#include <IL.h>

#include "imgui.h"

using namespace IL;

class ExampleLayer : public IL::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraPosition({0.0f, 0.0f, 0.0f}), m_CameraRotation(0.0f)
	{
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

	void OnUpdate(Timer dt) override
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

		Renderer::Submit(m_Shader, m_VertexArray);

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(IL::Event& e) override 
	{

	}

private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray; // it contains the vertexBuffers and indexBuffer
	std::shared_ptr<Camera> m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraRotation;

	float camMoveSpeed = 2.0f;
	float camRotationSpeed = 80.0f;
};

class SandBox : public IL::Application
{
public: 
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}

	~SandBox()
	{
	}

};

IL::Application* IL::CreateApplication()
{
	return new SandBox;
}
