#pragma once
#include <chrono>

namespace GTDLi
{
	using USEC = std::chrono::steady_clock::time_point;

	class Timestep
	{
	public:
		Timestep()
			: m_Time(std::chrono::steady_clock::now()) , m_Duration(0.0f), p_Duration(&m_Duration)
		{
			// Open to do stuff here
		}

		// Updates the m_Duration pointer value. This gets the duration since last time it was called.
		// It sets the currently held time.
		void UpdateDT()
		{
			USEC old = m_Time;
			m_Time = std::chrono::steady_clock::now();
			m_Duration = std::chrono::duration_cast<std::chrono::duration<double>>(m_Time - old).count();
		}

		// Get seconds (float) since last time the function was called
		// Sets current time to now
		double GetDTSec() 
		{ 
			UpdateDT();
			return m_Duration;
		}

		const double* getPDTSec()
		{
			return p_Duration;
		}

		// Operators
		// @TODO: Could get rid of these as I don't see a use for them quite yet
		friend bool operator<(const Timestep& l, const Timestep& r)
		{
			return l.m_Time < r.m_Time;
		}

		friend bool operator>(const Timestep& l, const Timestep& r)
		{
			return l.m_Time > r.m_Time;
		}

		friend bool operator<=(const Timestep& l, const Timestep& r)
		{
			return l.m_Time <= r.m_Time;
		}

		friend bool operator>=(const Timestep& l, const Timestep& r)
		{
			return l.m_Time >= r.m_Time;
		}

		friend bool operator==(const Timestep& l, const Timestep& r)
		{
			return l.m_Time == r.m_Time;
		}

	public:
		// This is used as the global pointer for durations
		// Moving components can use this to calculate the elapsed time between updates
		const double* p_Duration;

	private:
		USEC m_Time; // @TODO Is there a default for this?
		double m_Duration;
	};
}