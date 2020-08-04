#pragma once

#ifdef IL_PLATFORM_WINDOWS

	extern IL::Application* IL::CreateApplication();

	int main(int argc, char** argv)
	{
		IL::Log::Init();
		IL_CORE_WARN("Initalization Log!");
		//int a = BIT(0), b = BIT(1), c = BIT(2), d = BIT(3);
		//IL_INFO("Welcome to IL engine!");

		auto app = IL::CreateApplication();
		app->Run();
		delete app;

		return 0;
	}

#endif