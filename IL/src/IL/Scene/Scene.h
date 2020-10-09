#pragma once

#include "entt.hpp"

#include "IL/Core/TimeStep.h"

namespace IL
{
	class Entity;
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(TimeStep dt);
	private:
		friend class Entity;
		entt::registry m_Registry;
	};

}