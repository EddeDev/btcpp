#pragma once

#ifdef BTCPP_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace btcppUtils {

	void SetTitle(const std::string& title)
	{
		SetConsoleTitleA(title.c_str());
	}

	void SetEcho(bool enabled)
	{
#ifdef BTCPP_PLATFORM_WINDOWS
		HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);

		DWORD mode;
		GetConsoleMode(handle, &mode);
		if (!enabled)
			mode &= ~ENABLE_ECHO_INPUT;
		else
			mode |= ENABLE_ECHO_INPUT;

		SetConsoleMode(handle, mode);
#endif
	}

}