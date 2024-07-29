#include "config.h"

void removePrefix(std::vector<std::string>& strings, const std::string& prefix)
{
	size_t prefixLength = prefix.length();
	for (auto& str : strings) {
		// SprawdŸ, czy ci¹g zaczyna siê od prefiksu
		if (str.compare(0, prefixLength, prefix) == 0) {
			// U¿yj substr, aby usun¹æ prefiks
			str = str.substr(prefixLength);
		}
	}
}

std::vector<std::string> split(std::string line, std::string delimiter)
{
	std::vector<std::string> split_line;

	size_t pos = 0;
	std::string token;
	while ((pos = line.find(delimiter)) != std::string::npos)
	{
		token = line.substr(0, pos);
		split_line.push_back(token);
		line.erase(0, pos + delimiter.length());
	}
	split_line.push_back(line);
	return split_line;
}
