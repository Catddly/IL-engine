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
		ImGui::Begin("Hello! Test!");
		ImGui::Text("Hello World!");
		ImGui::ArrowButton("1", 1);
		ImGui::ArrowButton("2", 2);
		ImGui::ArrowButton("3", 3);
		ImGui::End();
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
