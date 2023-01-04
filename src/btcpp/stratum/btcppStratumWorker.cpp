#include "btcppPCH.h"
#include "btcppStratumWorker.h"

#include "btcppStratumErrorCode.h"

#include "btcpp/btcppStringUtils.h"
#include "btcpp/debugging/btcppScopedTimer.h"

static btcppStratumReplyMessageResult s_NullMessage = {};

btcppStratumWorker::btcppStratumWorker(const btcppStratumWorkerCreateInfo& createInfo)
	: m_Username(createInfo.Username), m_Password(createInfo.Password), m_ErrorCallback(createInfo.ErrorCallback)
{
	btcppTCPClientCreateInfo clientCreateInfo;
	clientCreateInfo.Address = createInfo.URL;
	clientCreateInfo.Port = std::to_string(createInfo.Port);

	m_Client = btcppRef<btcppTCPClient>::Create(clientCreateInfo);
	BTCPP_CHECK(m_Client);

	if (m_Client->GetConnectionStatus() != ConnectionStatus::Connected)
		return;

	if (!Subscribe())
		return;

	if (!Authorize())
		return;
}

btcppStratumWorker::~btcppStratumWorker()
{

}

bool btcppStratumWorker::UpdateMessageQueue()
{
#if 0
	ScopedTimer timer("Stratum Worker - UpdateMessageQueue", TimeUnit::Milliseconds);
#endif

	if (!m_Client->Peek())
		return false;

	bool status = false;
	std::string reply;
	while (m_Client->ReadLine(reply) && !reply.empty())
	{
		auto j = json::parse(reply);

		btcppStratumReplyMessageResult result = {};
		result.JSON = j;
		result.Message = ParseMessage(result.JSON);

#if 0
		BTCPP_INFO_CATEGORY("Stratum Worker", "Message Queue Reply:\n{0}", j.dump(4));
#endif

		m_ReplyMessageQueue.push_back(result);
		reply.clear();

		status = true;
	}
	return status;
}

bool btcppStratumWorker::HasMessage(btcppStratumMessage message) const
{
	for (auto it = m_ReplyMessageQueue.begin(); it != m_ReplyMessageQueue.end(); it++)
	{
		if (it->Message == message)
			return true;
	}
	return false;
}

bool btcppStratumWorker::HasAnyMessages() const
{
	return !m_ReplyMessageQueue.empty();
}

const btcppStratumReplyMessageResult& btcppStratumWorker::GetMessage(btcppStratumMessage message) const
{
	for (auto it = m_ReplyMessageQueue.begin(); it != m_ReplyMessageQueue.end(); it++)
	{
		if (it->Message == message)
			return *it;
	}
	return s_NullMessage;
}

const btcppStratumReplyMessageResult& btcppStratumWorker::GetFirstMessage() const
{
	return m_ReplyMessageQueue.front();
}

const btcppStratumReplyMessageResult& btcppStratumWorker::PopMessage()
{
	if (m_ReplyMessageQueue.empty())
		return s_NullMessage;

	auto& message = m_ReplyMessageQueue.front();
	m_ReplyMessageQueue.pop_front();
	return message;
}

bool btcppStratumWorker::Subscribe()
{
#if 1
	btcppScopedTimer timer("Stratum Worker - Subscribe", btcppTimeUnit::Milliseconds);
#endif

	BTCPP_INFO_CATEGORY("Stratum Worker", "Subscribing...");
	auto result = Invoke(btcppStratumMessage::Subscribe, { "btcppMiner" });

	auto& replyResult = result.Reply.JSON["result"];
	m_Extranonce = replyResult[1].get<std::string>();
	m_Extranonce2Size = replyResult[2].get<int32_t>();

	return true;
}

bool btcppStratumWorker::Authorize()
{
#if 1
	btcppScopedTimer timer("Stratum Worker - Authorize", btcppTimeUnit::Milliseconds);
#endif

	BTCPP_INFO_CATEGORY("Stratum Worker", "Authorizing...");
	auto result = Invoke(btcppStratumMessage::Authorize, { m_Username, m_Password });

	bool status = result.Reply.JSON["result"].get<bool>();
	if (!status)
	{
#if 0
		BTCPP_ERROR_CATEGORY("Stratum Worker", "Authorize failed");
#endif
		m_ErrorCallback(btcppLoginFailureType::WrongUsername);
	}

	return status;
}

btcppStratumMessageResult btcppStratumWorker::Invoke(btcppStratumMessage message, const std::initializer_list<std::string>& params)
{
	btcppStratumMessageResult result = {};
	result.ID = GetCounterID();

	json j;
	j["id"] = result.ID;
	j["method"] = btcppUtils::StratumMessageToString(message);
	j["params"] = params;

	result.JSONString = j.dump(0);
	result.JSONString.erase(std::remove(result.JSONString.begin(), result.JSONString.end(), '\n'), result.JSONString.cend());
	result.JSONString = btcppUtils::ReplaceAll(result.JSONString, ",", ", ");
	result.JSONString += '\n';

	if (m_Client->Send(result.JSONString))
		BTCPP_INFO_CATEGORY("Stratum Worker", "Successfully sent message to server ('{0}')", j["method"]);
	else
		BTCPP_ERROR_CATEGORY("Stratum Worker", "Failed to send message to server...");

	result.Reply = WaitForID(result.ID);
	if (result.Reply.Message == btcppStratumMessage::None)
		BTCPP_ERROR_CATEGORY("Stratum Worker", "Failed to get server reply...");

#if 0
	BTCPP_TRACE_CATEGORY("Stratum Worker", "Reply Message:\n{0}", result.Reply.JSON.dump(4));
#endif

	if (result.Reply.JSON.contains("error"))
	{
		const auto& error = result.Reply.JSON["error"];
		btcppStratumErrorCode errorCode = btcppStratumErrorCode::None;
		if (!error.is_null())
		{
			errorCode = static_cast<btcppStratumErrorCode>(error[0].get<int32_t>());
			BTCPP_ERROR_CATEGORY("Stratum Worker", "Error: {0} (ID: {1})", error[1].get<std::string>(), result.ID);
			BTCPP_CHECK(false);
		}
	}

	return result;
}

btcppStratumMessage btcppStratumWorker::ParseMessage(const json& j)
{
	if (j.contains("method"))
	{
		auto& method = j["method"];
		if (method.is_null())
			return btcppStratumMessage::Unknown;

		return btcppUtils::StratumMessageFromString(method.get<std::string>());
	}

	return btcppStratumMessage::Unknown;
}

btcppStratumReplyMessageResult btcppStratumWorker::WaitForID(int32_t id)
{
	std::string reply;
	while (m_Client->ReadLine(reply) && !reply.empty())
	{
		auto j = json::parse(reply);

		btcppStratumReplyMessageResult result = {};
		result.JSON = j;
		result.Message = ParseMessage(result.JSON);

		if (!j["id"].is_null() && j["id"].get<int32_t>() == id)
			return result;

		m_ReplyMessageQueue.push_back(result);
		reply.clear();
	}

	return {};
}

int32_t btcppStratumWorker::GetCounterID() const
{
	int32_t result = m_Counter;
	m_Counter++;
	return result;
}