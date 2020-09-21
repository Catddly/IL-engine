#pragma once

#include <IL.h>

using namespace IL;

class Sandbox2DLayer : public Layer
{
public:
	Sandbox2DLayer();
	virtual ~Sandbox2DLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDeatch() override;

	virtual void OnUpdate(TimeStep dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& e) override;
private:
	Ref<OrthographicCameraController> m_CameraController;

	Ref<Texture2D> m_Texture1, m_Texture2;

	glm::vec4 m_SquareColor = glm::vec4(0.3f, 0.3f, 0.9f, 1.0f);
	float m_SquareRotation = 0.0f;
	float m_Depth = 0.0f;
	float m_TilingFactor = 1.0f;
	float m_Alpha = 1.0f;
};