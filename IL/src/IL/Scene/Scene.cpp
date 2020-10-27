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
		entity.AddComponent<TransformComponent>();
		auto& tagCom = entity.AddComponent<TagComponent>();
		tagCom.Tag = name.empty() ? "Default" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(TimeStep dt)
	{
		Camera* mainCamera = nullptr;
		glm::mat4 transform = glm::mat4(1.0f);
		{
			// find the primary camera
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [trans, cam] = view.get<TransformComponent, CameraComponent>(entity);

				if (cam.Primary)
				{
					mainCamera = &cam.camera;
					transform = trans.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), transform);

			// Render sprite
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(trans.GetTransform(), sprite.Color);
			}

			// Instantiate and update all native scripts
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
			{
				//TODO: move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.instantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(dt);
			});

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		
		// Resize non-fixed aspect ratio camera
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cam = view.get<CameraComponent>(entity);

			if (!cam.FixedAspectRatio)
			{
				cam.camera.SetViewportSize(width, height);
			}
		}
	}

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
}