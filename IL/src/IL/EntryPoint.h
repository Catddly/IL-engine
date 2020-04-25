#pragma once

#ifdef IL_PLATFORM_WINDOWS

	extern IL::Application* IL::CreateApplication();

	int main(int argc, char** argv)
	{
		IL::Log::Init();
		IL_CORE_WARN("Initalization Log!");
		int a = 2;
		IL_INFO("Welcome to IL engine! Var={0}", a);

		auto app = IL::CreateApplication();
		app->Run();
		delete app;

		return 0;
	}

#endif