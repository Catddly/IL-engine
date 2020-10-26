#pragma once

#include <glm/glm.hpp>

#include "IL/Input/Input.h"

#include "IL/Scene/Components.h"
#include "IL/Scene/Entity.h"
#include "IL/Scene/ScriptableEntity.h"

namespace IL
{

	class SquareAnimated : public SciptableEntity
	{
	public:
		glm::vec3 beginTranslation;
		float clock;

		void OnCreate()
		{
			auto& scale = GetComponent<TransformComponent>().Scale;
			scale.x = 0.4f;
			scale.y = 0.4f;
			scale.z = 0.4f;
			beginTranslation = GetComponent<TransformComponent>().Translation;
			clock = 0.0f;
		}

		void OnUpdate(TimeStep dt)
		{
			auto& translation = GetComponent<TransformComponent>().Translation;
			float speed = 3.0f;
			float amplitude = 1.5f;

			// 3D sphere
			//translation.x = glm::sin(clock) * glm::cos(clock) * amplitude;
			//translation.y = glm::sin(clock) * glm::sin(clock) * amplitude;
			//translation.z = glm::cos(clock) * amplitude;
			// 2D sphere
			translation.x = beginTranslation.x + glm::cos(clock) * amplitude;
			translation.y = beginTranslation.y + glm::sin(clock) * amplitude;
			clock += dt * speed;
		}

		std::string FeedbackScriptName() { return "SquareAnimated"; }
	};

	class CameraController : public SciptableEntity
	{
	public:
		void OnUpdate(TimeStep dt) override
		{
			auto& translation = GetComponent<TransformComponent>().Translation;
			float speed = 5.0f;

			if (Input::IsKeyPressed(IL_KEY_W))
				translation.y += speed * dt;
			if (Input::IsKeyPressed(IL_KEY_A))
				translation.x -= speed * dt;
			if (Input::IsKeyPressed(IL_KEY_S))
				translation.y -= speed * dt;
			if (Input::IsKeyPressed(IL_KEY_D))
				translation.x += speed * dt;
			if (Input::IsKeyPressed(IL_KEY_LEFT_CONTROL))
				translation.z -= 1.5f * speed * dt;
			if (Input::IsKeyPressed(IL_KEY_SPACE))
				translation.z += 1.5f * speed * dt;
		}

		std::string FeedbackScriptName() { return "CameraController"; }
	};

}

