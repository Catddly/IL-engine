#pragma once

#ifdef IL_PLATFORM_WINDOWS

	extern IL::Application* IL::CreateApplication();

	int main(int argc, char** argv)
 	{
		IL::Log::Init();
		IL_CORE_WARN("Initalization Log!");
		IL_INFO("Press esc to close! Press t to display window feedback detail!");

		IL_PROFILE_BEGIN_SESSION("Startup", "profile/ILProfile-Startup.json");
		auto app = IL::CreateApplication();
		IL_PROFILE_END_SESSTION();

		IL_PROFILE_BEGIN_SESSION("Runtime", "profile/ILProfile-Runtime.json");
		app->Run();
		IL_PROFILE_END_SESSTION();

		IL_PROFILE_BEGIN_SESSION("Shutdown", "profile/ILProfile-Shutdown.json");
		delete app;
		IL_PROFILE_END_SESSTION();

		return 0;
	}

#endif