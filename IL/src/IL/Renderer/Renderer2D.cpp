#include "ilpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IL
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices  = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr  = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> QuadTextures;
		uint32_t CurTextureIndex;

		glm::vec4 QuadVertexPos[4];
	};

	static Renderer2DData s_Data;

	Renderer2D::SceneData* Renderer2D::s_SceneData = new Renderer2D::SceneData();

	void Renderer2D::Init()
	{
		IL_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		if (!s_SceneData->EnableBatchRendering)
		{
			// one square
			float vertices[4 * 5] = {
				-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
				 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f,  0.0f, 1.0f, 0.0f
			};
			s_Data.QuadVertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
			s_Data.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" },
			});
		} 
		else
		{
			s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxQuads];
			s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
			s_Data.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float,  "a_TexIndex" },
				{ ShaderDataType::Float,  "a_TilingFactor" }
			});
		}

		// Initialize draw call data of OpenGL
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		// IndexBuffer
		if (!s_SceneData->EnableBatchRendering)
		{
			uint32_t indices[6] = { 0, 1, 2, 0, 3, 1 };
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
			s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		}
		else
		{
			uint32_t* indices = new uint32_t[s_Data.MaxIndices];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 0;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 1;

				offset += 4;
			}
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_Data.MaxIndices);
			s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		}

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&textureData, sizeof(uint32_t));

		s_Data.QuadShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.QuadShader->Bind();
		if (!s_SceneData->EnableBatchRendering)
		{
			s_Data.QuadShader->SetInt("u_Texture", 0);
		}
		else
		{
			int32_t slots[32];
			for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
				slots[i] = i;
			s_Data.QuadShader->SetIntArray("u_Textures", slots, s_Data.MaxTextureSlots);
			// Textures[0] should be the white texture
			s_Data.QuadTextures[0] = s_Data.WhiteTexture;

			s_Data.QuadVertexPos[0] = { -0.5f, -0.5f,  0.0f, 1.0f };
			s_Data.QuadVertexPos[1] = {  0.5f,  0.5f,  0.0f, 1.0f };
			s_Data.QuadVertexPos[2] = { -0.5f,  0.5f,  0.0f, 1.0f };
			s_Data.QuadVertexPos[3] = {  0.5f, -0.5f,  0.0f, 1.0f };
		
		}
	}

	void Renderer2D::Shutdown()
	{
		IL_PROFILE_FUNCTION();

		delete s_SceneData;
	}

	void Renderer2D::BeginScene(const Ref<OrthographicCamera>& camera, bool EnableBatchRendering)
	{
		IL_PROFILE_FUNCTION();

		s_SceneData->EnableBatchRendering = EnableBatchRendering;
		Init();

		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		s_Data.QuadShader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CurTextureIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		IL_PROFILE_FUNCTION();

		s_Data.QuadShader->SetBool("u_BatchRender", s_SceneData->EnableBatchRendering);
		if (s_SceneData->EnableBatchRendering)
		{
			uint32_t byteCount = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, byteCount);

			Flush();
		}
	}

	void Renderer2D::Flush()
	{
		// batch render only draw once
		for (uint32_t i = 0; i < s_Data.CurTextureIndex; i++)
		{
			s_Data.QuadTextures[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
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

		if (!s_SceneData->EnableBatchRendering)
		{
			s_Data.WhiteTexture->Bind();

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // T S
			s_Data.QuadShader->SetFloat4("u_Color", color);
			s_Data.QuadShader->SetMat4("u_Transform", transform);

			s_Data.QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		}
		else
		{
			const float whiteTextureIndex = 0.0f;

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[0];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[1];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[2];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[3];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float& tilingFactor, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture, tilingFactor, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float& tilingFactor, const glm::vec4& color)
	{
		IL_PROFILE_FUNCTION();

		if (!s_SceneData->EnableBatchRendering)
		{
			texture->Bind();

			s_Data.QuadShader->SetFloat4("u_Color", color);
			s_Data.QuadShader->SetFloat("u_UVScaling", tilingFactor);

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // T S
			s_Data.QuadShader->SetMat4("u_Transform", transform);

			s_Data.QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		}
		else
		{
			float textureIndex = 0.0f;
			// if there are more than one submitted texture
			for (uint32_t i = 1; i < s_Data.CurTextureIndex; i++)
			{
				// if the texture had been submitted
				if (*s_Data.QuadTextures[i].get() == *texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			// new texture
			if (textureIndex == 0.0f)
			{
				textureIndex = s_Data.CurTextureIndex;
				s_Data.QuadTextures[s_Data.CurTextureIndex] = texture;
				s_Data.CurTextureIndex++;
			}

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[0];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[1];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[2];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[3];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
		}
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& color)
	{
		IL_PROFILE_FUNCTION();

		if (!s_SceneData->EnableBatchRendering)
		{
			s_Data.WhiteTexture->Bind();

			s_Data.QuadShader->SetFloat4("u_Color", color);

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // TRS
			s_Data.QuadShader->SetMat4("u_Transform", transform);

			s_Data.QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		}
		else
		{
			const float whiteTextureIndex = 0.0f;

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[0];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[1];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[2];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[3];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = whiteTextureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
		}
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float& tilingFactor /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tilingFactor, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float& tilingFactor /*= 1.0f*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
	{
		IL_PROFILE_FUNCTION();


		if (!s_SceneData->EnableBatchRendering)
		{
			texture->Bind();

			s_Data.QuadShader->SetFloat4("u_Color", color);
			s_Data.QuadShader->SetFloat("u_UVScaling", tilingFactor);

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // TRS
			s_Data.QuadShader->SetMat4("u_Transform", transform);

			s_Data.QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		}
		else
		{
			float textureIndex = 0.0f;
			// if there are more than one submitted texture
			for (uint32_t i = 1; i < s_Data.CurTextureIndex; i++)
			{
				// if the texture had been submitted
				if (*s_Data.QuadTextures[i].get() == *texture.get())
				{
					textureIndex = (float)i;
					break;
				}
			}

			// new texture
			if (textureIndex == 0.0f)
			{
				textureIndex = s_Data.CurTextureIndex;
				s_Data.QuadTextures[s_Data.CurTextureIndex] = texture;
				s_Data.CurTextureIndex++;
			}

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[0];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[1];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[2];
			s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPos[3];
			s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;

			s_Data.QuadIndexCount += 6;
		}
	}

}