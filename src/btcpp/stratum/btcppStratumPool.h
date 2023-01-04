#pragma once

#include "btcpp/btcppPool.h"

#include "btcppStratumWorker.h"

class btcppStratumPool : public btcppPool
{
public:
	btcppStratumPool(const btcppPoolCreateInfo& createInfo);
	virtual ~btcppStratumPool();

	virtual bool HasNewBlock() override;
	virtual btcppBlock GetNewBlock() override;

	virtual void SubmitBlock(const btcppBlock& block) override;

	virtual btcppRef<btcppTCPClient> GetClient() const override { return m_Worker->GetClient(); }
private:
	void FlushMessageQueue();
private:
	btcppRef<btcppStratumWorker> m_Worker;
	std::string m_Username;
};