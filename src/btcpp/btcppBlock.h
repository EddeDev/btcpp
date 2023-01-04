#pragma once

#include <vector>

struct btcppBlock
{
	std::string JobID;
	std::string PreviousHash;
	std::string Coinbase1;
	std::string Coinbase2;
	std::vector<std::string> MerkleBranch;
	std::string Version;
	std::string NBits;
	std::string NTime;
	bool CleanJobs = false;

	std::string Extranonce1;
	int32_t Extranonce2Size;

	std::string Extranonce2;
	std::string Nonce;

	bool IsValid() const
	{
		return !JobID.empty();
	}
};