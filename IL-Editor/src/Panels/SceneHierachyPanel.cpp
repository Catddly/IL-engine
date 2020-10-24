#include "SceneHierachyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "IL/Scene/Components.h"

#include "../Scripts/Scripts.h"

namespace IL
{
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float defaultValue = 0.0f, 
		float speed = 0.05f, float columnWidth = 100.0f)
	{
		ImGuiIO io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		// unique ID
		ImGui::PushID(label.c_str());

		// first column for label
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Button X begin
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.71f, 0.75f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.41f, 0.71f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.71f, 0.75f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = defaultValue;
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		// Button X end

		// Button Y begin
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.596f, 0.984f, 0.596f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.596f, 0.984f, 0.596f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = defaultValue;
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		// Button Y end

		// Button Z begin
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.529f, 0.808f, 0.98f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.749f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.529f, 0.808f, 0.98f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = defaultValue;
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		// Button Z end

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

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

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectedEntity)
		{
			DrawProperties(m_SelectedEntity);

			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{	
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
				bool opened = ImGui::TreeNodeEx("Add Component", flags);

				if (opened)
				{
					if (ImGui::MenuItem("Sprite Renderer"))
					{
						if (!m_SelectedEntity.HasComponent<SpriteRendererComponent>())
							m_SelectedEntity.AddComponent<SpriteRendererComponent>();
					}
					if (ImGui::MenuItem("Camera"))
					{
						if (!m_SelectedEntity.HasComponent<CameraComponent>())
							m_SelectedEntity.AddComponent<CameraComponent>();
					}

					bool SecondOpened = ImGui::TreeNodeEx("Scripts", ImGuiTreeNodeFlags_OpenOnArrow);;
					if (SecondOpened)
					{
						if (ImGui::MenuItem("Camera Controller"))
						{
							if (!m_SelectedEntity.HasComponent<NativeScriptComponent>())
								m_SelectedEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
						}
						if (ImGui::MenuItem("Animated Square"))
						{
							if (!m_SelectedEntity.HasComponent<NativeScriptComponent>())
								m_SelectedEntity.AddComponent<NativeScriptComponent>().Bind<SquareAnimated>();
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierachyPanel::DrawEntityTag(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((entity == m_SelectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectedEntity == entity)
				m_SelectedEntity = { };
		}
	}

	template <typename T, typename Fn>
	static void DrawComponent(const std::string& componentName, Entity entity, Fn uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;

			auto& component = entity.GetComponent<T>();
			auto regionAvailX = ImGui::GetContentRegionAvail().x;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2, 2 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, componentName.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(regionAvailX - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSetting");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSetting"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierachyPanel::DrawProperties(Entity entity)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
		{
			DrawVec3Control("Position", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation, 0.0f, 2.0f);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component)
		{
			auto& cameraComponent = component;
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
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});

		DrawComponent<NativeScriptComponent>("Native Script", entity, [](NativeScriptComponent& component)
		{
			auto& instance = component.Instance;
			ImGui::Text(instance->FeedbackScriptName().c_str());
		});
	}

}