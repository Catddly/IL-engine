#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace IL
{

	class IL_API Log
	{
	public:
		Log() = default;
		~Log() = default;

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()   { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// core logger macros
#define IL_CORE_TRACE(...) IL::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define IL_CORE_INFO(...)  IL::Log::GetCoreLogger()->info(__VA_ARGS__)
#define IL_CORE_WARN(...)  IL::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define IL_CORE_ERROR(...) IL::Log::GetCoreLogger()->error(__VA_ARGS__)
#define IL_CORE_FATAL(...) IL::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client logger macros
#define IL_TRACE(...)	   IL::Log::GetClientLogger()->trace(__VA_ARGS__)
#define IL_INFO(...)	   IL::Log::GetClientLogger()->info(__VA_ARGS__)
#define IL_WARN(...)	   IL::Log::GetClientLogger()->warn(__VA_ARGS__)
#define IL_ERROR(...)	   IL::Log::GetClientLogger()->error(__VA_ARGS__)
#define IL_FATAL(...)	   IL::Log::GetClientLogger()->fatal(__VA_ARGS__)
