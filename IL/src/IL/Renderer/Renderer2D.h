#pragma once

#include <glm/glm.hpp>

#include "Camera/OrthographicCamera.h"

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

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	};

}