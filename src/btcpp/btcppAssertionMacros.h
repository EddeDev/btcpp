#pragma once

#include "logging/btcppLogger.h"

#if defined(BTCPP_PLATFORM_WINDOWS)
	#define BTCPP_PLATFORM_BREAK() __debugbreak()
#elif defined(BTCPP_PLATFORM_LINUX)
	#inclide <signal.h>
	#define BTCPP_PLATFORM_BREAK() raise(SIGTRAP)
#else
	#define BTCPP_PLATFORM_BREAK()
#endif

#ifdef BTCPP_BUILD_DEBUG
	#define BTCPP_ENABLE_ASSERTS
	#define BTCPP_ENABLE_CHECKS
#endif

#ifdef BTCPP_ENABLE_CHECKS
	#define BTCPP_CHECK(expression) if (!(expression)) BTCPP_PLATFORM_BREAK()
#else
	#define BTCPP_CHECK(expression)
#endif

#ifdef BTCPP_ENABLE_ASSERTS
	#define BTCPP_ASSERT(expression, ...) \
	{ \
		if (!(expression)) \
		{ \
			BTCPP_LOG_PREFIX(Error, "Assertion failed", __VA_ARGS__); \
			BTCPP_PLATFORM_BREAK(); \
		} \
	}
#else
	#define BTCPP_ASSERT(expression, ...)
#endif

#ifndef BTCPP_BUILD_SHIPPING
	#define BTCPP_VERIFY(expression, ...) \
	{ \
		if (!(expression)) \
		{ \
			BTCPP_LOG_PREFIX(Error, "Verify failed", __VA_ARGS__); \
			BTCPP_PLATFORM_BREAK(); \
		} \
	}
#else
	#define BTCPP_VERIFY(expression, ...)
#endif
