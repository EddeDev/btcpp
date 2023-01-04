#pragma once

#include "btcppTimeUnit.h"

#include "btcpp/logging/btcppLogMacros.h"
#include "btcpp/btcppAssertionMacros.h"

#include <chrono>

struct btcppTimer
{
public:
	btcppTimer(btcppTimeUnit unit = btcppTimeUnit::Seconds)
		: m_Unit(unit)
	{
		Reset();
	}

	void Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	float Elapsed()
	{
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start);
		switch (m_Unit)
		{
		case btcppTimeUnit::Seconds:      return static_cast<float>(duration.count()) * 0.001f * 0.001f * 0.001f;
		case btcppTimeUnit::Milliseconds: return static_cast<float>(duration.count()) * 0.001f * 0.001f;
		case btcppTimeUnit::Nanoseconds:  return static_cast<float>(duration.count());
		}
		BTCPP_VERIFY(false, "Unknown time unit");
		return 0.0f;
	}
private:
	btcppTimeUnit m_Unit;

	std::chrono::high_resolution_clock::time_point m_Start;
};