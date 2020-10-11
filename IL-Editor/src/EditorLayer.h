#pragma once

#include <IL.h>

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
		Ref<OrthographicCameraController> m_CameraController;

		Ref<Texture2D> m_Texture1, m_Texture2, m_TileMap;
		Ref<SubTexture2D> m_TreeTex, m_SnowmanTex, m_FloorTex;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_ScriptEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		glm::vec4 m_SquareColor = glm::vec4(0.3f, 0.3f, 0.9f, 1.0f);
		float m_SquareRotation = 0.0f;
		float m_Depth = 0.0f;
		float m_TilingFactor = 1.0f;
		float m_Alpha = 1.0f;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};

}