#pragma once

#include "btcpp/btcppRefCounting.h"

#include "btcppLogVerbosity.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

class btcppLogger
{
public:
	static void Init();
	static void Shutdown();

	static btcppShared<spdlog::logger>& GetLogger() { return s_Logger; }
private:
	static btcppShared<spdlog::logger> s_Logger;
};

#define BTCPP_LOG_ARGS(verbosity, ...) btcppLogger::GetLogger()->log(btcppUtils::LogVerbosityToSpdlogLevel(btcppLogVerbosity::##verbosity), __VA_ARGS__)
#define BTCPP_LOG_PREFIX(verbosity, prefix, ...) btcppLogger::GetLogger()->log(btcppUtils::LogVerbosityToSpdlogLevel(btcppLogVerbosity::##verbosity), "{0}: {1}", prefix, fmt::format(__VA_ARGS__))
#define BTCPP_LOG_CATEGORY(verbosity, categoryName, ...) btcppLogger::GetLogger()->log(btcppUtils::LogVerbosityToSpdlogLevel(btcppLogVerbosity::##verbosity), "[{0}]: {1}", categoryName, fmt::format(__VA_ARGS__))