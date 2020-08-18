#include <IL.h>

#include "imgui.h"

class ExampleLayer : public IL::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
	}

	void OnImGuiRender() override
	{
		//ImGui::Begin("Hello! Test!");
		//ImGui::Text("Hello World!");
		//ImGui::End();
	}

	void OnEvent(IL::Event& e) override 
	{
	}
};

class SandBox : public IL::Application
{
public: 
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}

	~SandBox()
	{
	}

};

IL::Application* IL::CreateApplication()
{
	return new SandBox;
}
