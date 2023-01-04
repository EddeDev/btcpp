#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

enum class btcppLogVerbosity : uint8_t
{
	None = 0,

	Trace,
	Info,
	Warning,
	Error,
	Critical
};

namespace btcppUtils {

	static const char* LogVerbosityToString(btcppLogVerbosity verbosity)
	{
		switch (verbosity)
		{
		case btcppLogVerbosity::None:     return "None";
		case btcppLogVerbosity::Trace:    return "Trace";
		case btcppLogVerbosity::Info:     return "Info";
		case btcppLogVerbosity::Warning:  return "Warning";
		case btcppLogVerbosity::Error:    return "Error";
		case btcppLogVerbosity::Critical: return "Critical";
		}
		return "UnknownVerbosity";
	}

	static btcppLogVerbosity LogVerbosityFromString(const std::string& verbosityString)
	{
		if (verbosityString == "Trace")
			return btcppLogVerbosity::Trace;
		else if (verbosityString == "Info")
			return btcppLogVerbosity::Info;
		else if (verbosityString == "Warning")
			return btcppLogVerbosity::Warning;
		else if (verbosityString == "Error")
			return btcppLogVerbosity::Error;
		else if (verbosityString == "Critical")
			return btcppLogVerbosity::Critical;
		return btcppLogVerbosity::None;
	}

	static spdlog::level::level_enum LogVerbosityToSpdlogLevel(btcppLogVerbosity verbosity)
	{
		switch (verbosity)
		{
		case btcppLogVerbosity::Trace:    return spdlog::level::trace;
		case btcppLogVerbosity::Info:     return spdlog::level::info;
		case btcppLogVerbosity::Warning:  return spdlog::level::warn;
		case btcppLogVerbosity::Error:    return spdlog::level::err;
		case btcppLogVerbosity::Critical: return spdlog::level::critical;
		}
		return spdlog::level::off;
	}

}
