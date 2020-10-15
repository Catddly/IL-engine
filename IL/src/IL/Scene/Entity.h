#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace IL
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene)
			:m_EntityHandle(entity), m_Scene(scene) {}
		Entity(const Entity&) = default;

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template <typename T, typename ...Args>
		T& AddComponent(Args&&... args)
		{
			//IL_CORE_ASSERT(HasComponent<T>(), "Already had a component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			IL_CORE_ASSERT(!HasComponent<T>(), "Do not had this component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template <typename T>
		void RemoveComponent()
		{
			IL_CORE_ASSERT(!HasComponent<T>(), "Do not had this component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }

		bool operator==(const Entity& rhs) const
		{
			return m_EntityHandle == rhs.m_EntityHandle && m_Scene == rhs.m_Scene;
		}

		bool operator!=(const Entity& rhs) const
		{
			return !(*this == rhs);
		}
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}
