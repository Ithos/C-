#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace StringFunctions
{
	class StringFunctions
	{
	public:
		static void splitString(const std::string & source, char separator, std::vector<std::string>& oVec)
		{
			std::string token;
			std::istringstream f(source);

			while (std::getline(f, token, separator))
			{
				oVec.push_back(trim(token));
			}
		}

		static std::string trim(const std::string& str)
		{
			size_t first = str.find_first_not_of(' ');
			if (std::string::npos == first)
			{
				return str;
			}
			size_t last = str.find_last_not_of(' ');
			return str.substr(first, (last - first + 1));
		}
	};
}
