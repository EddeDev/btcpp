#pragma once

#include "btcpp/btcppAssertionMacros.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class btcppStratumMessage : uint8_t
{
	None = 0,
	Unknown,

	// Client to server
	Authorize,
	Capabilities,
	GetTransactions,
	Submit,
	Subscribe,
	SuggestDifficulty,
	SuggestTarget,

	// Server to client
	GetVersion,
	Reconnect,
	ShowMessage,
	Notify,
	SetDifficulty,
	SetExtranonce,
	SetGoal
};

struct btcppStratumReplyMessageResult
{
	json JSON;
	btcppStratumMessage Message = btcppStratumMessage::None;
};

struct btcppStratumMessageResult
{
	int32_t ID = 0;
	std::string JSONString;
	btcppStratumReplyMessageResult Reply;
};

namespace btcppUtils {

	static const char* StratumMessageToString(btcppStratumMessage message)
	{
		switch (message)
		{
		case btcppStratumMessage::None:              return "none";
		case btcppStratumMessage::Unknown:           return "unknown";
		case btcppStratumMessage::Authorize:         return "mining.authorize";
		case btcppStratumMessage::Capabilities:      return "mining.capabilities";
		case btcppStratumMessage::GetTransactions:   return "mining.get_transactions";
		case btcppStratumMessage::Submit:            return "mining.submit";
		case btcppStratumMessage::Subscribe:         return "mining.subscribe";
		case btcppStratumMessage::SuggestDifficulty: return "mining.suggest_difficulty";
		case btcppStratumMessage::SuggestTarget:     return "mining.suggest_target";
		case btcppStratumMessage::GetVersion:        return "client.get_version";
		case btcppStratumMessage::Reconnect:         return "client.reconnect";
		case btcppStratumMessage::ShowMessage:       return "client.show_message";
		case btcppStratumMessage::Notify:            return "mining.notify";
		case btcppStratumMessage::SetDifficulty:     return "mining.set_difficulty";
		case btcppStratumMessage::SetExtranonce:     return "mining.set_extranonce";
		case btcppStratumMessage::SetGoal:           return "mining.set_goal";
		}
		BTCPP_VERIFY(false, "Unknown btcppStratumMessage");
		return "";
	}

	static btcppStratumMessage StratumMessageFromString(const std::string& messageString)
	{
#define MAP(message, str) if (messageString == str) return btcppStratumMessage::message

		MAP(None, "none");
		MAP(Unknown, "unknown");
		MAP(Authorize, "mining.authorize");
		MAP(Capabilities, "mining.capabilities");
		MAP(GetTransactions, "mining.get_transactions");
		MAP(Submit, "mining.submit");
		MAP(Subscribe, "mining.subscribe");
		MAP(SuggestDifficulty, "mining.suggest_difficulty");
		MAP(SuggestTarget, "mining.suggest_target");
		MAP(GetVersion, "client.get_version");
		MAP(Reconnect, "client.reconnect");
		MAP(ShowMessage, "client.show_message");
		MAP(Notify, "mining.notify");
		MAP(SetDifficulty, "mining.set_difficulty");
		MAP(SetExtranonce, "mining.set_extranonce");
		MAP(SetGoal, "mining.set_goal");

		return btcppStratumMessage::None;
	}

}
