#include "ilpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGlShader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace IL
{
	
	struct Renderer2DStorage
	{
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_SquareShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->m_VertexArray = VertexArray::Create();
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
		s_Data->m_VertexArray->AddVertexBuffer(vertexBuffer);

		// IndexBuffer
		uint32_t indices[6] = { 0, 1, 2, 0, 3, 1 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->m_VertexArray->SetIndexBuffer(indexBuffer);

		s_Data->m_SquareShader = Shader::Create("assets/shaders/Square.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_SquareShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_SquareShader)->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_SquareShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPortSize(0, 0, width, height);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_SquareShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_SquareShader)->UploadUniformFloat4("u_SquareColor", color);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_SquareShader)->UploadUniformMat4("u_Transform", scale);

		s_Data->m_VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->m_VertexArray);
	}

}