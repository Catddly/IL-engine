#pragma once

#include <glm/glm.hpp>
#include <string>

#include "ScriptableEntity.h"

namespace IL
{

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4 & color)
			:Color(color) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& matrix)
			:Transform(matrix) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct NativeScriptComponent
	{
		SciptableEntity* Instance = nullptr;

		using InstantiateScript = SciptableEntity*(*)();
		using DestroyScript = void(*)(NativeScriptComponent*);

		InstantiateScript instantiateScript = nullptr;
		DestroyScript destroyScript = nullptr;

		template <typename T>
		void Bind()
		{
			instantiateScript = []() { return static_cast<SciptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}