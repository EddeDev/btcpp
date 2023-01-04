#pragma once

#include "btcpp/btcppAssertionMacros.h"

enum class btcppProtocolType : uint8_t
{
	None = 0,

	Stratum = 1,
	HTTP = 2
};

class btcppProtocol
{
public:
	static void Set(btcppProtocolType type) { s_CurrentProtocolType = type; }
	static btcppProtocolType Get() { return s_CurrentProtocolType; }
private:
	inline static btcppProtocolType s_CurrentProtocolType = btcppProtocolType::Stratum;
};

namespace btcppUtils {

	static const char* ProtocolTypeToString(btcppProtocolType type)
	{
		switch (type)
		{
		case btcppProtocolType::Stratum: return "Stratum";
		case btcppProtocolType::HTTP:    return "HTTP";
		}
		BTCPP_VERIFY(false, "Unknown btcppProtocolType");
		return "";
	}

}