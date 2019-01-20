#include "Textures.h"

TexturesFiles::Textures* TexturesFiles::Textures::_mpInstance = nullptr;
const char TexturesFiles::TexturesFilesConstants::SEPARATOR = '=';

TexturesFiles::Textures * TexturesFiles::Textures::InitInstance(const std::string& texFolder, const std::string& texConfFile)
{
	if(_mpInstance == nullptr)
		_mpInstance = new Textures(texFolder, texConfFile);

	return _mpInstance;
}

void TexturesFiles::Textures::DestroyInstance()
{
	if (_mpInstance != nullptr)
	{
		delete _mpInstance;
		_mpInstance = nullptr;
	}
}

const std::string & TexturesFiles::Textures::GetTextureDir(const std::string & key)
{
	assert(mTextures.find(key) != mTextures.end() && "Texture not found");
	return mTextures[key];
}

TexturesFiles::Textures::Textures(const std::string& texFolder, const std::string& texConfFile): mTexFolder(texFolder), mTexConf(texConfFile)
{
	init();
}

void TexturesFiles::Textures::init()
{
	assert(ConfigFileFunctions::ConfigFileFunctions::ReadConfigFileIntoStringMap(mTexFolder, mTexConf, TexturesFiles::TexturesFilesConstants::SEPARATOR, mTextures) == 
		ConfigFileFunctions::ReadConfigErrorCodes::SUCCESS);
}

TexturesFiles::Textures::~Textures()
{
}
