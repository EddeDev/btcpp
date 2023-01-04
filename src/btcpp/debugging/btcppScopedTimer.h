#pragma once

#include "btcppTimeUnit.h"

#include "btcpp/logging/btcppLogMacros.h"

#include <chrono>

struct btcppScopedTimer
{
public:
	btcppScopedTimer(std::string_view name, btcppTimeUnit unit = btcppTimeUnit::Seconds)
		: m_Name(name), m_Unit(unit)
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	~btcppScopedTimer()
	{
		m_Stop = std::chrono::high_resolution_clock::now();

		switch (m_Unit)
		{
		case btcppTimeUnit::Seconds:
		{
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(m_Stop - m_Start);
			BTCPP_INFO_ARGS("[TIMER]: {0}: {1} seconds", m_Name, duration.count());
			break;
		}
		case btcppTimeUnit::Milliseconds:
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_Stop - m_Start);
			BTCPP_INFO_ARGS("[TIMER]: {0}: {1}ms", m_Name, duration.count());
			break;
		}
		case btcppTimeUnit::Nanoseconds:
		{
			auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(m_Stop - m_Start);
			BTCPP_INFO_ARGS("[TIMER]: {0}: {1}ns", m_Name, duration.count());
			break;
		}
		}
	}
private:
	std::string_view m_Name;
	btcppTimeUnit m_Unit;

	std::chrono::high_resolution_clock::time_point m_Start;
	std::chrono::high_resolution_clock::time_point m_Stop;
};
