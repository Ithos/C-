#pragma once

#ifndef __CONFIGURATION_CONFMANAGER_H
#define __CONFIGURATION_CONFMANAGER_H

#include "StringFunctions.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <map>

namespace Configuration
{

	struct ConfigurationManagerConstants
	{
		static const std::string CONFIG_FILE;
		static const std::string VERTEX_SHADER_FOLDER;
		static const std::string FRAGMENT_SHADER_FOLDER;
		static const std::string CONFIG_FRAGMENT_SHADER;
		static const std::string CONFIG_VERTEX_SHADER;
		static const std::string TEXTURES_FOLDER;
		static const std::string CONFIG_TEXTURES;
		static const char SEPARATOR;
	};

	class ConfigurationManager
	{
	public:
		static ConfigurationManager* GetInstance();
		static void DestroyInstance();

		const std::string& getVertexShaderFolder() { return _mVertexShaderFolder; }
		const std::string& getFragmentShaderFolder() { return _mFragmentShaderFolder; }
		const std::string& getVertexShaderConfig() { return _mVertexShaderConfig; }
		const std::string& getFragmentShaderConfig() { return _mFragmentShaderConfig; }
		const std::string& getTexturesFolder() { return _mTexturesFolder; }
		const std::string& getTexturesConfig() { return _mTexturesConfig; }

	private:
		static ConfigurationManager* _mpInstance;
		std::string _mVertexShaderFolder;
		std::string _mFragmentShaderFolder;
		std::string _mVertexShaderConfig;
		std::string _mFragmentShaderConfig;
		std::string _mTexturesFolder;
		std::string _mTexturesConfig;
		std::map <std::string, std::string* > _mConfigMap;

		ConfigurationManager();
		virtual ~ConfigurationManager();

		void init();
		void readConfigFile(std::map <std::string, std::string* > & map);

	};
}
#endif