#pragma once

#ifdef IL_PLATFORM_WINDOWS
	#ifdef IL_BUILD_DLL
		#define IL_API __declspec(dllexport)
	#else
		#define IL_API __declspec(dllimport)
	#endif
#else
	#error IL only support windows!
#endif