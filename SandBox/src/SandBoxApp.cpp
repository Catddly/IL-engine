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
		IL_TRACE("Layer:{0} {1}", GetName(), e);
	}
};

class SandBox : public IL::Application
{
public: 
	SandBox()
	{
		PushLayer(new ExampleLayer());
<<<<<<< HEAD
		PushOverlay(new IL::ImGuiLayer());
=======
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
	}

	~SandBox()
	{
	}

};

IL::Application* IL::CreateApplication()
{
	return new SandBox;
}
