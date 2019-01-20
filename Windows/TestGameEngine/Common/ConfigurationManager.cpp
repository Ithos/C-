#include "ConfigurationManager.h"

namespace Configuration
{

	ConfigurationManager* ConfigurationManager::_mpInstance = nullptr;

	const std::string ConfigurationManagerConstants::CONFIG_FILE = "../settings/config";
	const std::string ConfigurationManagerConstants::VERTEX_SHADER_FOLDER = "VERTEX_SHADER_FOLDER";
	const std::string ConfigurationManagerConstants::FRAGMENT_SHADER_FOLDER = "FRAGMENT_SHADER_FOLDER";
	const std::string ConfigurationManagerConstants::CONFIG_FRAGMENT_SHADER = "CONFIG_FRAGMENT_SHADER";
	const std::string ConfigurationManagerConstants::CONFIG_VERTEX_SHADER = "CONFIG_VERTEX_SHADER";
	const std::string ConfigurationManagerConstants::TEXTURES_FOLDER = "TEXTURES_FOLDER";
	const std::string ConfigurationManagerConstants::CONFIG_TEXTURES = "CONFIG_TEXTURES";
	const char ConfigurationManagerConstants::SEPARATOR = '=';

	ConfigurationManager * ConfigurationManager::GetInstance()
	{
		if (_mpInstance == nullptr)
		{
			_mpInstance = new ConfigurationManager();
		}

		return _mpInstance;
	}

	void ConfigurationManager::DestroyInstance()
	{
		if (_mpInstance != nullptr)
		{
			delete _mpInstance;
			_mpInstance = nullptr;
		}
	}

	ConfigurationManager::ConfigurationManager()
	{
		init();
	}

	ConfigurationManager::~ConfigurationManager()
	{
	}

	void ConfigurationManager::init()
	{
		_mConfigMap[ConfigurationManagerConstants::VERTEX_SHADER_FOLDER] = &_mVertexShaderFolder;
		_mConfigMap[ConfigurationManagerConstants::FRAGMENT_SHADER_FOLDER] = &_mFragmentShaderFolder;
		_mConfigMap[ConfigurationManagerConstants::CONFIG_FRAGMENT_SHADER] = &_mFragmentShaderConfig;
		_mConfigMap[ConfigurationManagerConstants::CONFIG_VERTEX_SHADER] = &_mVertexShaderConfig;
		_mConfigMap[ConfigurationManagerConstants::TEXTURES_FOLDER] = &_mTexturesFolder;
		_mConfigMap[ConfigurationManagerConstants::CONFIG_TEXTURES] = &_mTexturesConfig;

		readConfigFile(_mConfigMap);
	}

	void ConfigurationManager::readConfigFile(std::map <std::string, std::string*> & map)
	{
		std::ifstream infile(ConfigurationManagerConstants::CONFIG_FILE);

		assert(infile.is_open() && "Config file not found");

		std::string line;
		while (std::getline(infile, line))
		{
			std::vector<std::string> tokenConfig;
			StringFunctions::StringFunctions::splitString(StringFunctions::StringFunctions::trim(line), ConfigurationManagerConstants::SEPARATOR, tokenConfig);

			auto it = map.find(tokenConfig[0]);

			if (it != map.end())
			{
				(*map[tokenConfig[0]]) = tokenConfig[1];
			}
		}
	}

}


