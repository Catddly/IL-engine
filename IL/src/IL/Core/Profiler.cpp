#include "ilpch.h"
#include "Profiler.h"

namespace IL
{

	void Profiler::Stop()
	{
		auto now = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_TimerStartTime).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();

		auto duration = end - start;
		m_IsStoped = true;

		m_ProfileResults.push_back({m_Description, duration * 0.001});
	}

	std::vector<ProfileResult> Profiler::m_ProfileResults(0);
}