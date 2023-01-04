#include "btcppPCH.h"
#include "btcppPool.h"
#include "btcppProtocol.h"
#include "btcppStringUtils.h"
#include "btcppConsoleUtils.h"

int32_t ParsePort(const std::string& url, size_t* offset = nullptr)
{
	size_t lastColon = url.find_last_of(':');
	if (lastColon == std::string::npos)
	{
		BTCPP_CHECK(false);
		return 0;
	}

	if (offset)
		*offset = lastColon;

	std::string portString = url.substr(lastColon + 1);
	return std::stoi(portString);
}

btcppProtocolType ParseProtocolType(const std::string& url, size_t* offset = nullptr)
{
	size_t found = url.find_first_of("://");
	if (found != std::string::npos)
	{
		bool stratumFound = false;
		bool httpFound = false;
		bool tcpFound = false;

		for (const auto& string : btcppUtils::SplitString(url.substr(0, found), "+"))
		{
			stratumFound |= btcppUtils::ContainsIgnoreCase(string, "stratum");
			httpFound |= btcppUtils::ContainsIgnoreCase(string, "http");
			tcpFound |= btcppUtils::ContainsIgnoreCase(string, "tcp");
		}

		if (offset)
			*offset = found + strlen("://");

		if (stratumFound)
		{
			BTCPP_CHECK(!httpFound);
			BTCPP_CHECK(tcpFound);

			if (tcpFound)
				return btcppProtocolType::Stratum;
		}

		if (httpFound)
		{
			BTCPP_CHECK(!stratumFound);
			BTCPP_CHECK(!tcpFound);

			return btcppProtocolType::HTTP;
		}
	}

	return btcppProtocolType::None;
}

int main()
{
	btcppCore::Init();
	btcppUtils::SetTitle("btcppMiner v1.0");

	std::string url;
	std::cout << "Pool URL: ";
	std::cin >> url;

	std::string username;
	std::cout << "Username: ";
	std::cin >> username;
	username += ".btcppWorker";

	std::string password;
	std::cout << "Password: ";
	btcppUtils::SetEcho(false);
	std::cin >> password;
	btcppUtils::SetEcho(true);
	std::cout << '\n';

	size_t protocolOffset = 0;
	auto protocolType = ParseProtocolType(url, &protocolOffset);
	BTCPP_CHECK(protocolType != btcppProtocolType::None);
	btcppProtocol::Set(protocolType);
	
	size_t portOffset = 0;
	int32_t port = ParsePort(url, &portOffset);

	btcppPoolCreateInfo poolCreateInfo;
	poolCreateInfo.URL = url.substr(protocolOffset, portOffset - protocolOffset);
	poolCreateInfo.Port = port;
	poolCreateInfo.Username = username;
	poolCreateInfo.Password = password;
	poolCreateInfo.ErrorCallback = [](auto failureType)
	{
		BTCPP_CHECK(false);
	};

	btcppRef<btcppPool> pool = btcppPool::Create(poolCreateInfo);
	
	btcppBlock currentBlock;

	if (pool->HasNewBlock())
		currentBlock = pool->GetNewBlock();

	bool running = true;
	while (running)
	{
	}

	btcppCore::Shutdown();
}