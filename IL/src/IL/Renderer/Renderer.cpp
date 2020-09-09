#include "ilpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGlShader.h"
#include "Renderer2D.h"

namespace IL
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
	{
		m_SceneData->m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPortSize(0, 0, width, height);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->m_ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

}
