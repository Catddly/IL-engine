#include "ilpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IL
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_Data;

	Renderer2D::SceneData* Renderer2D::s_SceneData = new Renderer2D::SceneData();

	void Renderer2D::Init()
	{
		IL_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->vertexArray = VertexArray::Create();
		// one square
		float vertices[4 * 5] = {
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
			{ ShaderDataType::Float2, "a_TexCoord" },
		};
		vertexBuffer->SetLayout(layout);

		// Initialize draw call data of OpenGL
		// VertexArray
		s_Data->vertexArray->AddVertexBuffer(vertexBuffer);

		// IndexBuffer
		uint32_t indices[6] = { 0, 1, 2, 0, 3, 1 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->vertexArray->SetIndexBuffer(indexBuffer);

		s_Data->shader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->shader->Bind();
		s_Data->shader->SetInt("u_Texture", 0);

		s_Data->whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data->whiteTexture->SetData(&data, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		IL_PROFILE_FUNCTION();

		delete s_Data;
		delete s_SceneData;
	}

	void Renderer2D::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		IL_PROFILE_FUNCTION();

		s_SceneData->m_ViewProjection = camera->GetViewProjectionMatrix();
		s_Data->shader->SetMat4("u_ViewProjection", s_SceneData->m_ViewProjection);
	}

	void Renderer2D::EndScene()
	{
		IL_PROFILE_FUNCTION();
	}

	void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
	{
		IL_PROFILE_FUNCTION();

		RenderCommand::SetViewPortSize(0, 0, width, height);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		IL_PROFILE_FUNCTION();

		s_Data->whiteTexture->Bind();

		s_Data->shader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // T S
		s_Data->shader->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float& uvScaling, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture, uvScaling, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float& uvScaling, const glm::vec4& color)
	{
		IL_PROFILE_FUNCTION();

		texture->Bind();

		s_Data->shader->SetFloat4("u_Color", color);
		s_Data->shader->SetFloat("u_UVScaling", uvScaling);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // T S
		s_Data->shader->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& color)
	{
		IL_PROFILE_FUNCTION();

		s_Data->whiteTexture->Bind();

		s_Data->shader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // TRS
		s_Data->shader->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float& uvScaling /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, texture, uvScaling , color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float& uvScaling /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		IL_PROFILE_FUNCTION();

		texture->Bind();

		s_Data->shader->SetFloat4("u_Color", color);
		s_Data->shader->SetFloat("u_UVScaling", uvScaling);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // TRS
		s_Data->shader->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

}