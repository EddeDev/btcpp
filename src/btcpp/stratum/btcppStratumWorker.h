#pragma once

#include "btcppStratumMessage.h"

#include "btcpp/btcppTCP.h"
#include "btcpp/btcppAuthentication.h"

struct btcppStratumWorkerCreateInfo
{
	std::string URL;
	int32_t Port;
	std::string Username;
	std::string Password;

	btcppLoginErrorCallback ErrorCallback;
};

class btcppStratumWorker : public btcppRefCounted
{
public:
	btcppStratumWorker(const btcppStratumWorkerCreateInfo& createInfo);
	virtual ~btcppStratumWorker();

	bool UpdateMessageQueue();
	bool HasMessage(btcppStratumMessage message) const;
	bool HasAnyMessages() const;
	const btcppStratumReplyMessageResult& GetMessage(btcppStratumMessage message) const;
	const btcppStratumReplyMessageResult& GetFirstMessage() const;
	const btcppStratumReplyMessageResult& PopMessage();

	const std::string& GetExtranonce() const { return m_Extranonce; }
	int32_t GetExtranonce2Size() const { return m_Extranonce2Size; }

	btcppStratumMessageResult Invoke(btcppStratumMessage message, const std::initializer_list<std::string>& params = {});

	btcppRef<btcppTCPClient> GetClient() const { return m_Client; }
private:
	bool Subscribe();
	bool Authorize();

	btcppStratumMessage ParseMessage(const json& j);

	btcppStratumReplyMessageResult WaitForID(int32_t id);
	int32_t GetCounterID() const;
private:
	btcppRef<btcppTCPClient> m_Client;
	std::deque<btcppStratumReplyMessageResult> m_ReplyMessageQueue;

	btcppLoginErrorCallback m_ErrorCallback;

	// Authorization
	std::string m_Username;
	std::string m_Password;

	std::string m_Extranonce;
	int32_t m_Extranonce2Size = 0;

	mutable int32_t m_Counter = 1;
};