#include "btcppPCH.h"
#include "btcppStratumPool.h"

btcppStratumPool::btcppStratumPool(const btcppPoolCreateInfo& createInfo)
	: m_Username(createInfo.Username)
{
	btcppStratumWorkerCreateInfo workerCreateInfo;
	workerCreateInfo.URL = createInfo.URL;
	workerCreateInfo.Port = createInfo.Port;
	workerCreateInfo.Username = createInfo.Username;
	workerCreateInfo.Password = createInfo.Password;
	workerCreateInfo.ErrorCallback = createInfo.ErrorCallback;

	m_Worker = btcppRef<btcppStratumWorker>::Create(workerCreateInfo);
}

btcppStratumPool::~btcppStratumPool()
{
}

bool btcppStratumPool::HasNewBlock()
{
	m_Worker->UpdateMessageQueue();
	return m_Worker->HasMessage(btcppStratumMessage::Notify);
}

btcppBlock btcppStratumPool::GetNewBlock()
{
	m_Worker->UpdateMessageQueue();

	btcppBlock result = {};
	auto& message = m_Worker->GetMessage(btcppStratumMessage::Notify);
	if (message.Message == btcppStratumMessage::Notify)
	{
		result.JobID = message.JSON["params"][0].get<std::string>();
		result.PreviousHash = message.JSON["params"][1].get<std::string>();
		result.Coinbase1 = message.JSON["params"][2].get<std::string>();
		result.Coinbase2 = message.JSON["params"][3].get<std::string>();
		for (size_t i = 0; i < message.JSON["params"][4].size(); i++)
			result.MerkleBranch.push_back(message.JSON["params"][4][i].get<std::string>());
		result.Version = message.JSON["params"][5].get<std::string>();
		result.NBits = message.JSON["params"][6].get<std::string>();
		result.NTime = message.JSON["params"][7].get<std::string>();
		result.CleanJobs = message.JSON["params"][8].get<bool>();
		result.Extranonce1 = m_Worker->GetExtranonce();
	}

	if (!result.IsValid())
		BTCPP_WARNING_ARGS("Returning non-valid BTC block...");
	return result;
}

void btcppStratumPool::SubmitBlock(const btcppBlock& block)
{
	m_Worker->Invoke(btcppStratumMessage::Submit, { m_Username, block.JobID, block.Extranonce2, block.NTime, block.Nonce });
}

void btcppStratumPool::FlushMessageQueue()
{
	while (m_Worker->GetFirstMessage().Message != btcppStratumMessage::Notify && m_Worker->HasAnyMessages())
	{
		auto& message = m_Worker->PopMessage();
		BTCPP_TRACE_ARGS("Reply Message:\n{0}", message.JSON.dump(4));
		BTCPP_CHECK(false);
	}
}