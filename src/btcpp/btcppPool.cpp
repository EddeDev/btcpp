#include "btcppPCH.h"
#include "btcppPool.h"

#include "btcppProtocol.h"
#include "Stratum/btcppStratumPool.h"

btcppRef<btcppPool> btcppPool::Create(const btcppPoolCreateInfo& createInfo)
{
	switch (btcppProtocol::Get())
	{
	case btcppProtocolType::Stratum: return btcppRef<btcppStratumPool>::Create(createInfo);
	}
	BTCPP_VERIFY(false, "Unknown protocol");
	return nullptr;
}