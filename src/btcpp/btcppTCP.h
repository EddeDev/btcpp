#pragma once

#include "btcpp/btcppAuthentication.h"

#include <winsock2.h>

struct btcppTCPClientCreateInfo
{
	std::string Address;
	std::string Port;

	bool IsDelayed = false;
	bool IsBlocking = true;

	btcppLoginErrorCallback ErrorCallback;
};

enum class ConnectionStatus : uint8_t
{
	None = 0,

	Connected,
	Disconnected,
	Closed
};

class btcppTCPClient : public btcppRefCounted
{
public:
	btcppTCPClient(const btcppTCPClientCreateInfo& createInfo);
	virtual ~btcppTCPClient();

	bool Peek();
	bool Send(const std::string& data);
	bool Receive(std::string& data, size_t size = 4096);
	bool ReadLine(std::string& data);

	void SetDelayed(bool enabled, bool forceSet = false);
	void SetBlocking(bool enabled, bool forceSet = false);

	ConnectionStatus GetConnectionStatus() const { return m_ConnectionStatus; }

	const std::string& GetAddress() const { return m_Address; }
	const std::string& GetPort() const { return m_Port; }
private:
	std::string m_Address;
	std::string m_Port;

	SOCKET m_Socket = INVALID_SOCKET;

	bool m_IsDelayed = false;
	bool m_IsBlocking = false;

	ConnectionStatus m_ConnectionStatus = ConnectionStatus::Disconnected;
};