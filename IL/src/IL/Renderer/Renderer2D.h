#pragma once

#include <glm/glm.hpp>

#include "Camera/OrthographicCamera.h"
#include "Texture.h"

namespace IL
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Ref<OrthographicCamera>& camera);
		static void EndScene();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
	private:
		struct SceneData
		{
			glm::mat4 m_ViewProjection;
		};

		static SceneData* s_SceneData;
	};

}