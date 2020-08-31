#pragma once

#ifdef IL_PLATFORM_WINDOWS

	extern IL::Application* IL::CreateApplication();

	int main(int argc, char** argv)
 	{
		IL::Log::Init();
		IL_CORE_WARN("Initalization Log!");
		IL_INFO("Press esc to close! Press t to display window feedback detail!");

		auto app = IL::CreateApplication();
		app->Run();
		delete app;

		return 0;
	}

#endif