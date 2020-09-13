#pragma once

#include "IL/Core/Core.h"

#include <string>
#include <fstream>

namespace IL
{
	struct ProfileResult
	{
		std::string Description;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) { }

		void BeginSession(const std::string& name, const std::string& filepath = "results.json");
		void EndSession();

		void WriteProfile(const ProfileResult& result);
		void WriteHeader();
		void WriteFooter();

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	};

	class Profiler
	{
	public:
		Profiler(const char* description)
			:m_Description(description), m_IsStoped(false)
		{
			m_TimerStartTime = std::chrono::high_resolution_clock::now();
		}

		~Profiler()
		{
			if (!m_IsStoped)
				Stop();
		}

		void Stop();
	private:
		const char* m_Description;
		bool m_IsStoped;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_TimerStartTime;
	};

// Unsolved problem PROFILING
#define IL_PROFILING 0

#if IL_PROFILING 	
	#define _STR_(x) #x
	#define _STR_LINE_(x) _STR_(x)
	#define _STR_LINE_NUM_ _STR_LINE_(__LINE__)
	#define PROFILER_SCOPE(name) Profiler profiler(name)
	#define PROFILER_FUNCTION() PROFILER_SCOPE(__FUNCSIG__)
#else
	#define _SL_(x) #x
	#define _STRLINE_(x) _SL_(x) #x
	#define _STR_LINE_ _STRLINE_(__LINE__)
	#define IL_PROFILE_SCOPE(name)
	#define IL_PROFILE_FUNCTION()
#endif 

#if IL_PROFILING 
#define IL_PROFILE_BEGIN_SESSION(name, filepath) ::IL::Instrumentor::Get().BeginSession(name, filepath)
#define IL_PROFILE_SCOPE(name) ::IL::Profiler profiler(name);
#define IL_PROFILE_FUNCTION() IL_PROFILE_SCOPE(__FUNCSIG__)
#define IL_PROFILE_END_SESSTION() ::IL::Instrumentor::Get().EndSession()
#else
#define IL_PROFILE_BEGIN_SESSION(name, filepath) 
#define IL_PROFILE_SCOPE(name)
#define IL_PROFILE_FUNCTION() 
#define IL_PROFILE_END_SESSTION() 
#endif 
}

