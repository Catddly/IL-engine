#include <IL.h>

class ExampleLayer : public IL::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override 
	{
		//IL_INFO("Example Layer : OnUpdate()");
	}

	void OnEvent(IL::Event& e) override 
	{
		//IL_TRACE("Layer:{0} {1}", GetName(), e);
	}
};

class SandBox : public IL::Application
{
public: 
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new IL::ImGuiLayer());
	}

	~SandBox()
	{
	}

};

IL::Application* IL::CreateApplication()
{
	return new SandBox;
}
