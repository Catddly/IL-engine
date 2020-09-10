#pragma once

#include "RenderCommand.h"

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Shader.h"
#include "Camera/Camera.h"

namespace IL
{

	class Renderer
	{
	public:
		// TODO
		static void BeginScene(const std::shared_ptr<Camera>& camera);
		static void EndScene();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& VertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static void Init();
		static void Shutdown();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 m_ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}
