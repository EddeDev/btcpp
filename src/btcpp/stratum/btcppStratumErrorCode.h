#pragma once

#include "btcpp/btcppAssertionMacros.h"

enum class btcppStratumErrorCode : int32_t
{
	None = 0,

	Other = 20,
	JobNotFound = 21,
	DuplicateShare = 22,
	LowDifficultyShare = 23,
	UnauthorizedWorker = 24,
	NotSubscribed = 25
};

namespace btcppUtils {

	static const char* StratumErrorCodeToString(btcppStratumErrorCode errorCode)
	{
		switch (errorCode)
		{
		case btcppStratumErrorCode::JobNotFound:        return "Job not found";
		case btcppStratumErrorCode::DuplicateShare:     return "Duplicate share";
		case btcppStratumErrorCode::LowDifficultyShare: return "Low difficulty share";
		case btcppStratumErrorCode::UnauthorizedWorker: return "Unauthorized worker";
		case btcppStratumErrorCode::NotSubscribed:      return "Not subscribed";
		}
		return "Other/Unknown";
	}

}