#pragma once

#include "IL/Core/Log.h"
#include "IL/Core/Core.h"
#include "IL/Scene/Scene.h"
#include "IL/Scene/Entity.h"

namespace IL
{

	class SceneHierachyPanel
	{
	public:
		SceneHierachyPanel() = default;
		SceneHierachyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityTag(Entity entity);
		void DrawProperties(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};

}