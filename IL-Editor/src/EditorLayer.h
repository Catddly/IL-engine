#pragma once

#include <IL.h>

#include "Panels/SceneHierachyPanel.h"

namespace IL
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDeatch() override;

		virtual void OnUpdate(TimeStep dt) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		Ref<Texture2D> m_Texture1, m_Texture2, m_TileMap;
		Ref<SubTexture2D> m_TreeTex, m_SnowmanTex, m_FloorTex;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntityL, m_SquareEntityR, m_SquareEntityT, m_SquareEntityB, m_SquareEntityFront, m_SquareEntityBack;
		Entity m_ScriptEntity;
		Entity m_FirstCameraEntity;
		Entity m_SecondCameraEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		// Panels
		SceneHierachyPanel m_SceneHierachyPanel;
	};

}