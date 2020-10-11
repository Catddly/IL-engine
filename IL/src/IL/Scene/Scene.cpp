#include "ilpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "IL/Renderer/Renderer2D.h"

namespace IL
{

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		auto& tagCom = entity.AddComponent<TagComponent>();
		tagCom.Tag = name.empty() ? "Default" : name;

		return entity;
	}

	void Scene::OnUpdate(TimeStep dt)
	{
		// Instantiate and update all native scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
		{
			//TODO: move to Scene::OnScenePlay
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity{ entity, this };
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(dt);
		});

		// Render sprite
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(trans.Transform, sprite.Color);
		}
	}

}