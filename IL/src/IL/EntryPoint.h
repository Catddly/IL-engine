#pragma once

#ifdef IL_PLATFORM_WINDOWS

	extern IL::Application* IL::CreateApplication();

	int main(int argc, char** argv)
	{
		printf("Welcome to IL engine!\n");

		auto app = IL::CreateApplication();
		app->Run();
		delete app;

		return 0;
	}

#endif