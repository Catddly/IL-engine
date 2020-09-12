#pragma once

#include "IL/Core/Core.h"

namespace IL
{
	struct ProfileResult
	{
		const char* Description;
		double Time;
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

		static const std::vector<ProfileResult>& GetResults() { return m_ProfileResults; };
		static void ClearResults() { m_ProfileResults.clear(); }
	private:
		const char* m_Description;
		bool m_IsStoped;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_TimerStartTime;

		static std::vector<ProfileResult> m_ProfileResults;
	};

#define PROFILER(name) Profiler profiler##__LINE__(name)
}

