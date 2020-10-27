#pragma once

#include "entt.hpp"

#include "IL/Core/TimeStep.h"
#include "IL/Events/Event.h"

namespace IL
{
	class Entity;
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(TimeStep dt);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;

		entt::registry m_Registry;
		uint32_t  m_ViewportWidth = 0, m_ViewportHeight = 0;
	};

}