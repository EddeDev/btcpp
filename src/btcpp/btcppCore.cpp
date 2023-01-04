#include "btcppPCH.h"
#include "btcppCore.h"

namespace btcppCore {

	void Init()
	{
		btcppLogger::Init();

		BTCPP_TRACE_ARGS("**************************************************");
		BTCPP_TRACE_ARGS("************ btcpp Bitcoin Miner v1.0 ************");
		BTCPP_TRACE_ARGS("**************************************************");
	}

	void Shutdown()
	{
		BTCPP_TRACE_ARGS("Shutting down...");

		btcppLogger::Shutdown();
	}

}