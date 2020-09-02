#pragma once

#include "IL/Core/Core.h"

namespace IL
{

	class Timer
	{
	public:
		Timer(const std::string& description)
		{
			m_TimerStartTime = std::chrono::high_resolution_clock::now();
			m_Description = description;
		}

		~Timer()
		{
			Stop();
		}

		void Stop() const
		{
			auto now = std::chrono::high_resolution_clock::now();

			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_TimerStartTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();

			auto duration = end - start;
			IL_CORE_INFO("{0} duration: ms({1}), us({2})", m_Description, duration * 0.001, duration);
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_TimerStartTime;
		std::string m_Description;
	};

}

