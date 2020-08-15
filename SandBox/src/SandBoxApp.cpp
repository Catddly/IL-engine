#include <IL.h>

class ExampleLayer : public IL::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		if (IL::Input::IsMouseButtonPressed(IL_MOUSE_BUTTON_1))
			IL_INFO("Mouse 1 is pressed!");
	}

	void OnEvent(IL::Event& e) override 
	{
		if (e.GetEventType() == IL::EventType::KeyPressed)
		{
			IL::KeyPressedEvent& event = (IL::KeyPressedEvent&)e;
			IL_TRACE("{0}", (char)event.GetKeyCode());
		}
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
