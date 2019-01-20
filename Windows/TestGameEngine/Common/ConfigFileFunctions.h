#pragma once
#ifndef CONFIGFILEFUNCTIONS_H
#define CONFIGFILEFUNCTIONS_H

#include "StringFunctions.h"
#include <string>
#include <map>
#include <fstream>

namespace ConfigFileFunctions
{
	enum class ReadConfigErrorCodes
	{
		SUCCESS,
		CONFIG_FILE_NOT_FOUND,
		DUPLICATE_KEY
	};
	

	class ConfigFileFunctions
	{
	public:
		static ReadConfigErrorCodes ReadConfigFileIntoStringMap(const std::string& folderPath, const std::string& confFile, char separator, std::map<std::string, std::string>& desMap)
		{
			std::ifstream infile(std::string(folderPath).append(confFile));
			if (!infile.is_open())
			{
				return ReadConfigErrorCodes::CONFIG_FILE_NOT_FOUND;
			}

			std::string line;
			while (std::getline(infile, line))
			{
				std::vector<std::string> tokenConfig;
				StringFunctions::StringFunctions::splitString(StringFunctions::StringFunctions::trim(line), separator, tokenConfig);

				if (!(desMap.find(tokenConfig[0]) == desMap.end()))
				{
					return ReadConfigErrorCodes::DUPLICATE_KEY;
				}

				desMap.insert(std::pair<std::string, std::string>(tokenConfig[0], std::string(folderPath).append(tokenConfig[1])));
			}

			return ReadConfigErrorCodes::SUCCESS;
		}

	};
}

#endif