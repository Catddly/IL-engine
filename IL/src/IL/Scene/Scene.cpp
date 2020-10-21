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
		Camera* mainCamera = nullptr;
		glm::mat4* transform = nullptr;
		{
			// find the primary camera
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [trans, cam] = view.get<TransformComponent, CameraComponent>(entity);

				if (cam.Primary)
				{
					mainCamera = &cam.camera;
					transform = &trans.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *transform);

			// Render sprite
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(trans.Transform, sprite.Color);
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

}