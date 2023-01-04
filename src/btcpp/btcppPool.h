#pragma once

#include "btcppBlock.h"
#include "btcppAuthentication.h"

#include "btcpp/btcppTCP.h"

struct btcppPoolCreateInfo
{
	std::string URL;
	int32_t Port;

	std::string Username;
	std::string Password;

	btcppLoginErrorCallback ErrorCallback;
};

class btcppPool : public btcppRefCounted
{
public:
	virtual bool HasNewBlock() = 0;
	virtual btcppBlock GetNewBlock() = 0;

	virtual void SubmitBlock(const btcppBlock& block) = 0;

	virtual btcppRef<btcppTCPClient> GetClient() const = 0;

	static btcppRef<btcppPool> Create(const btcppPoolCreateInfo& createInfo);
};