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
	ShaderLibrary m_ShaderLibrary;
	Ref<Shader> m_SquareShader;

	Ref<VertexArray> m_VertexArray; // it contains the vertexBuffers and indexBuffer
	Ref<OrthographicCameraController> m_CameraController;

	glm::vec4 m_SquareColor = glm::vec4(0.3f, 0.3f, 0.9f, 1.0f);
};