#pragma once

#ifndef TEXTURESCONFIGURATION_H
#define TEXTURESCONFIGURATION_H

#include "ConfigFileFunctions.h"
#include <map>
#include <cassert>


namespace TexturesFiles
{
	struct TexturesFilesConstants
	{
		static const char SEPARATOR;
	};

	class Textures
	{
	public:
		static Textures* GetActiveInstance() { return _mpInstance; }
		static Textures* InitInstance(const std::string& texFolder, const std::string& texConfFile);
		static void DestroyInstance();

		const std::string& GetTextureDir(const std::string& key);

	private:
		static Textures* _mpInstance;
		std::map<std::string, std::string> mTextures;
		std::string mTexFolder;
		std::string mTexConf;

		Textures() {}
		Textures(const std::string& texFolder, const std::string& texConfFile);
		void init();
		virtual ~Textures();
	};
}
#endif