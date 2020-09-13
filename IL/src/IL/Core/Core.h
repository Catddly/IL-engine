#pragma once

#include <memory>

#ifdef IL_PLATFORM_WINDOWS
	#ifdef IL_DYNAMIC_LINK
		#ifdef IL_BUILD_DLL
			#define IL_API __declspec(dllexport)
		#else
			#define IL_API __declspec(dllimport)
		#endif
	#else
		#define IL_API
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

namespace IL
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}