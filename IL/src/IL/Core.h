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

#ifdef IL_ENABLE_ASSERTS
	#define IL_ASSERT(x, ...)      { if (!(x)) { IL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define IL_CORE_ASSERT(x, ...) { if (!(x)) { IL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else 
	#define IL_ASSERT(x, ...)
	#define IL_CORE_ASSERT(x, ...)
#endif 

#define BIT(x) (1 << x)

#define IL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
