#include "btcppPCH.h"
#include "btcppTCP.h"

#include <ws2tcpip.h>

static const WORD s_WinSockVersion = MAKEWORD(2, 2);

btcppTCPClient::btcppTCPClient(const btcppTCPClientCreateInfo& createInfo)
	: m_Address(createInfo.Address), m_Port(createInfo.Port)
{
	WSADATA wsaData = {};
	if (int32_t errorCode = WSAStartup(s_WinSockVersion, &wsaData); errorCode != 0)
		BTCPP_VERIFY(false, "[TCP Client]: WSAStartup failed with error: {0}", errorCode);

	addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo* result = nullptr;
	if (int32_t errorCode = getaddrinfo(createInfo.Address.c_str(), createInfo.Port.c_str(), &hints, &result); errorCode != 0)
	{
#if 0
		BTCPP_VERIFY(false, "[TCP Client]: getaddrinfo failed with error: {0}", errorCode);
#endif
		createInfo.ErrorCallback(btcppLoginFailureType::WrongURLOrPort);
	}

	for (addrinfo* it = result; it != nullptr; it = it->ai_next)
	{
		m_Socket = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
		BTCPP_VERIFY(m_Socket != INVALID_SOCKET, "[TCP Client]: socket failed with error: {0}", WSAGetLastError());

		if (connect(m_Socket, it->ai_addr, static_cast<int32_t>(it->ai_addrlen)) == SOCKET_ERROR)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(result);

	SetDelayed(createInfo.IsDelayed, true);
	SetBlocking(createInfo.IsBlocking, true);

	if (m_Socket == INVALID_SOCKET)
	{
		m_ConnectionStatus = ConnectionStatus::Disconnected;
		BTCPP_CHECK(false);
	}
	else
	{
		m_ConnectionStatus = ConnectionStatus::Connected;
	}
}

btcppTCPClient::~btcppTCPClient()
{
	closesocket(m_Socket);
	WSACleanup();
}

bool btcppTCPClient::Peek()
{
	SetBlocking(false);
	char buffer;
	bool result = recv(m_Socket, &buffer, 1, MSG_PEEK) > 0;
	SetBlocking(true);
	return result;
}

bool btcppTCPClient::Send(const std::string& data)
{
	if (m_ConnectionStatus != ConnectionStatus::Connected)
		return false;

	int32_t result = send(m_Socket, data.c_str(), static_cast<int32_t>(data.size()), 0);
	if (result == SOCKET_ERROR)
	{
		BTCPP_ERROR_CATEGORY("TCP Client", "send failed with error: {0}", WSAGetLastError());
		return false;
	}

	BTCPP_CHECK(result == static_cast<int32_t>(data.size()));
	return true;
}

bool btcppTCPClient::Receive(std::string& data, size_t size)
{
	if (m_ConnectionStatus != ConnectionStatus::Connected)
		return false;

	std::vector<char> buffer(size + 1);

	size_t count = recv(m_Socket, buffer.data(), static_cast<int32_t>(size), 0);
	if (count == std::string::npos)
	{
		int32_t errorCode = WSAGetLastError();
		if (errorCode == WSAEWOULDBLOCK)
			BTCPP_VERIFY(false, "A non-blocking socket operation could not be completed immediately.");
	}

	if (count > 0)
	{
		buffer[count] = '\0';
		data = buffer.data();

		return true;
	}

	if (count == 0)
		m_ConnectionStatus = ConnectionStatus::Closed;
	else
		BTCPP_ERROR_CATEGORY("TCP Client", "recv failed with error: {0}", WSAGetLastError());

	return false;
}

bool btcppTCPClient::ReadLine(std::string& data)
{
	if (m_ConnectionStatus != ConnectionStatus::Connected)
		return false;

	char buffer;
	std::string line;
	line.reserve(1024);

	int32_t count = 1;
	while (count > 0)
	{
		count = recv(m_Socket, &buffer, 1, 0);
		if (count == std::string::npos)
		{
			int32_t errorCode = WSAGetLastError();
			if (errorCode == WSAEWOULDBLOCK)
				BTCPP_VERIFY(false, "A non-blocking socket operation could not be completed immediately.");
			return false;
		}

		if (count < 0)
			return false;

		if (count > 0)
		{
			if (buffer != '\n')
				line.push_back(buffer);
			else
				break;
		}

		if (count == 0)
		{
			m_ConnectionStatus = ConnectionStatus::Closed;
			return false;
		}
	}

	data = line;
	return true;
}

void btcppTCPClient::SetDelayed(bool enabled, bool forceSet)
{
	if (m_IsDelayed != enabled || forceSet)
	{
		uint32_t noDelay = enabled ? 0 : 1;
		setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, sizeof(uint32_t));

		m_IsDelayed = enabled;
	}
}

void btcppTCPClient::SetBlocking(bool enabled, bool forceSet)
{
	if (m_IsBlocking != enabled || forceSet)
	{
		unsigned long nonBlocking = 1;
		unsigned long blocking = 0;
		ioctlsocket(m_Socket, FIONBIO, enabled ? &blocking : &nonBlocking);

		m_IsBlocking = enabled;
	}
}