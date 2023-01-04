#pragma once

struct btcppAuthenticationDetails
{
	std::string URL;
	int32_t Port;
	std::string Username;
	std::string Password;
	std::string WorkerName;
};

enum class btcppLoginFailureType : uint8_t
{
	None = 0,

	WrongUsername,
	WrongURLOrPort,

	InvalidPort,
	URLFieldIsEmpty,
	PortFieldIsEmpty,
	UsernameFieldIsEmpty,
	PasswordFieldIsEmpty,
	WorkerNameFieldIsEmpty
};

struct btcppLoginFailureMessage
{
	btcppLoginFailureType Type;
};

using btcppLoginErrorCallback = std::function<void(btcppLoginFailureType)>;