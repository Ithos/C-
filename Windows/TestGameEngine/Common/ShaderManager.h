#pragma once

#ifndef __SHADERFILES_SHADERMANAGER_H
#define __SHADERFILES_SHADERMANAGER_H

#include "ConfigFileFunctions.h"
#include "Shader.h"
#include <iostream>
#include <cassert>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

namespace ShaderFiles
{
	struct ShaderManagerConstants
	{
		static const std::string TEST_VERTEX_SHADER;
		static const std::string TEST_FRAGMENT_SHADER;
		static const char SEPARATOR;
	};

	class ShaderManager
	{
	public:
		static ShaderManager* GetActiveInstance() { return _mpInstance; }
		static ShaderManager* GetInstance(const std::string& vertexFolder, const std::string& fragmentFolder, const std::string& vertexConf, const std::string& fragmentConf);
		static void DestroyInstance();

		void LoadShader(const std::string& shader);
		void UnloadShader(const std::string& shader);
		void UnloadAll();
		bool IsLoaded(const std::string& shader);

		const std::string& GetLoadedShaderContent(const std::string& shader);
		Shader& GetLoadedShader(const std::string& shader);

	private:
		static ShaderManager* _mpInstance;
		std::map<std::string, Shader*> _mLoadedShaders;
		std::map<std::string, std::string> _mShadersDirs;
		std::string _mVertexFolder;
		std::string _mFragmentFolder;
		std::string _mVertexConf;
		std::string _mFragmentConf;

		ShaderManager() {};
		ShaderManager(const std::string& vertexFolder, const std::string& fragmentFolder, const std::string& vertexConf, const std::string& fragmentConf);
		virtual ~ShaderManager();

		void init();
	};
}
#endif
