#pragma once

#include "btcppLogger.h"

#if !defined(BTCPP_BUILD_SHIPPING)
#define BTCPP_TRACE_ENABLED
#endif

#ifdef BTCPP_TRACE_ENABLED
#define BTCPP_TRACE_ARGS(...) BTCPP_LOG_ARGS(Trace, __VA_ARGS__)
#define BTCPP_TRACE_CATEGORY(category, ...) BTCPP_LOG_CATEGORY(Trace, category, __VA_ARGS__)
#define BTCPP_TRACE_PREFIX(prefix, ...) BTCPP_LOG_PREFIX(Trace, prefix, __VA_ARGS__)
#else
#define BTCPP_TRACE_ARGS(...)
#define BTCPP_TRACE_CATEGORY(category, ...)
#define BTCPP_TRACE_PREFIX(prefix, ...)
#endif

#define BTCPP_INFO_ARGS(...)     BTCPP_LOG_ARGS(Info,     __VA_ARGS__)
#define BTCPP_WARNING_ARGS(...)  BTCPP_LOG_ARGS(Warning,  __VA_ARGS__)
#define BTCPP_ERROR_ARGS(...)    BTCPP_LOG_ARGS(Error,    __VA_ARGS__)
#define BTCPP_CRITICAL_ARGS(...) BTCPP_LOG_ARGS(Critical, __VA_ARGS__)

#define BTCPP_INFO_CATEGORY(category, ...)     BTCPP_LOG_CATEGORY(Info,     category, __VA_ARGS__)
#define BTCPP_WARNING_CATEGORY(category, ...)  BTCPP_LOG_CATEGORY(Warning,  category, __VA_ARGS__)
#define BTCPP_ERROR_CATEGORY(category, ...)    BTCPP_LOG_CATEGORY(Error,    category, __VA_ARGS__)
#define BTCPP_CRITICAL_CATEGORY(category, ...) BTCPP_LOG_CATEGORY(Critical, category, __VA_ARGS__)

#define BTCPP_INFO_PREFIX(prefix, ...)     BTCPP_LOG_PREFIX(Info,     prefix, __VA_ARGS__)
#define BTCPP_WARNING_PREFIX(prefix, ...)  BTCPP_LOG_PREFIX(Warning,  prefix, __VA_ARGS__)
#define BTCPP_ERROR_PREFIX(prefix, ...)    BTCPP_LOG_PREFIX(Error,    prefix, __VA_ARGS__)
#define BTCPP_CRITICAL_PREFIX(prefix, ...) BTCPP_LOG_PREFIX(Critical, prefix, __VA_ARGS__)