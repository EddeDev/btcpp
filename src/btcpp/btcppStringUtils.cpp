#include "btcppPCH.h"
#include "btcppStringUtils.h"

namespace btcppUtils {

	std::vector<std::string> SplitString(std::string s, const std::string& delimiter)
	{
		size_t begin = 0, end, delimiterLength = delimiter.length();
		std::string token;
		std::vector<std::string> result;

		while ((end = s.find(delimiter, begin)) != std::string::npos)
		{
			token = s.substr(begin, end - begin);
			begin = end + delimiterLength;
			result.push_back(token);
		}

		result.push_back(s.substr(begin));
		return result;
	}

	std::string ReplaceAll(std::string string, const std::string& from, const std::string& to)
	{
		size_t begin = 0;
		while ((begin = string.find(from, begin)) != std::string::npos)
		{
			string.replace(begin, from.length(), to);
			begin += to.length();
		}
		return string;
	}

	bool ContainsIgnoreCase(const std::string& string, const std::string& searchString)
	{
		return ToLowerString(string) == ToLowerString(searchString);
	}

	std::string ToLowerString(std::string string)
	{
		std::transform(string.begin(), string.end(), string.begin(), [](uint8_t c)
		{
			return std::tolower(c);
		});
		return string;
	}

}