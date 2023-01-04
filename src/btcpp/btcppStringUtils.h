#pragma once

namespace btcppUtils {

	std::vector<std::string> SplitString(std::string s, const std::string& delimiter);
	std::string ReplaceAll(std::string string, const std::string& from, const std::string& to);

	bool ContainsIgnoreCase(const std::string& string, const std::string& searchString);
	std::string ToLowerString(std::string string);

}