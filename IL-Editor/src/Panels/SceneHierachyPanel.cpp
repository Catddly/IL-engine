#include "SceneHierachyPanel.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "IL/Scene/Components.h"

namespace IL
{
	SceneHierachyPanel::SceneHierachyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierachyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierachyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityTag(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = { };

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectedEntity)
			DrawProperties(m_SelectedEntity);

		ImGui::End();
	}

	void SceneHierachyPanel::DrawEntityTag(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((entity == m_SelectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (opened)
			ImGui::TreePop();
	}

	void SceneHierachyPanel::DrawProperties(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.05f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& cam = cameraComponent.camera;
				bool& fixedAspectRatio = cameraComponent.FixedAspectRatio;
				bool& isPrimary = cameraComponent.Primary;

				const char* projectionTypeStrings[] = { "Pespective", "Orthographic" };
				const char* currProjectionTypeString = projectionTypeStrings[(int)cam.GetProjectionType()];

				if (ImGui::BeginCombo("ProjectionType", currProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], &isSelected))
						{
							currProjectionTypeString = projectionTypeStrings[i];
							cam.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (cam.GetProjectionType() == SceneCamera::ProjectionType::Pespective)
				{
					ImGui::Checkbox("Primary", &isPrimary);

					auto pespectiveYFOV = glm::degrees(cam.GetPespectiveYFOV());
					if (ImGui::DragFloat("FOV", &pespectiveYFOV, 0.05f))
						cam.SetPespectiveYFOV(glm::radians(pespectiveYFOV));

					auto pespectiveNear = cam.GetPespectiveNearClip();
					if (ImGui::DragFloat("Near", &pespectiveNear, 0.05f))
						cam.SetPespectiveNearClip(pespectiveNear);

					auto pespectiveFar = cam.GetPespectiveFarClip();
					if (ImGui::DragFloat("Far", &pespectiveFar, 0.05f))
						cam.SetPespectiveFarClip(pespectiveFar);

					ImGui::Checkbox("FixedAspectRatio", &fixedAspectRatio);
				}

				if (cam.GetProjectionType() == SceneCamera::ProjectionType::Orthograghic)
				{
					ImGui::Checkbox("Primary", &isPrimary);

					auto orthoSize = cam.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize, 0.05f))
						cam.SetOrthographicSize(orthoSize);

					auto orthoNear = cam.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear, 0.05f))
						cam.SetOrthographicNearClip(orthoNear);

					auto orthoFar = cam.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar, 0.05f))
						cam.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("FixedAspectRatio", &fixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}
	}

}