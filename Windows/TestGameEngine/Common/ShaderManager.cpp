#include "ShaderManager.h"

namespace ShaderFiles
{
	ShaderManager* ShaderManager::_mpInstance = nullptr;
	const std::string ShaderManagerConstants::TEST_VERTEX_SHADER = "TEST_VERTEX_SHADER";
	const std::string ShaderManagerConstants::TEST_FRAGMENT_SHADER = "TEST_FRAGMENT_SHADER";
	const char ShaderManagerConstants::SEPARATOR = '=';

	ShaderManager * ShaderManager::GetInstance(const std::string& vertexFolder, const std::string& fragmentFolder, const std::string& vertexConf, const std::string& fragmentConf)
	{
		if (_mpInstance == nullptr)
		{
			_mpInstance = new ShaderManager(vertexFolder, fragmentFolder, vertexConf, fragmentConf);
		}

		return _mpInstance;
	}

	void ShaderManager::DestroyInstance()
	{
		if (_mpInstance != nullptr)
		{
			delete _mpInstance;
			_mpInstance = nullptr;
		}
	}

	void ShaderManager::LoadShader(const std::string & shader)
	{
		assert(_mLoadedShaders.find(shader) == _mLoadedShaders.end() && "Shader already loaded");
		assert(_mShadersDirs.find(shader) != _mShadersDirs.end() && "Shader path not found");

		_mLoadedShaders.insert(std::pair<std::string, Shader*>(shader, new Shader(_mShadersDirs[shader])));
	}

	void ShaderManager::UnloadShader(const std::string & shader)
	{
		assert(_mLoadedShaders.find(shader) != _mLoadedShaders.end() && "Shader not loaded");
		delete _mLoadedShaders[shader];
		_mLoadedShaders.erase(shader);
	}

	void ShaderManager::UnloadAll()
	{
		for (auto iter = _mLoadedShaders.begin(); iter != _mLoadedShaders.end(); ++iter)
		{
			delete (*iter).second;
		}

		_mLoadedShaders.clear();
	}

	bool ShaderManager::IsLoaded(const std::string & shader)
	{
		return _mLoadedShaders.find(shader) != _mLoadedShaders.end();
	}

	const std::string & ShaderManager::GetLoadedShaderContent(const std::string & shader)
	{
		return GetLoadedShader(shader).getContent();
	}

	Shader& ShaderManager::GetLoadedShader(const std::string & shader)
	{
		assert(_mLoadedShaders.find(shader) != _mLoadedShaders.end() && "Shader not loaded");

		return (*_mLoadedShaders[shader]);
	}

	ShaderManager::ShaderManager(const std::string& vertexFolder, const std::string& fragmentFolder, const std::string& vertexConf, const std::string& fragmentConf) :
		_mVertexFolder(vertexFolder), _mFragmentFolder(fragmentFolder), _mVertexConf(vertexConf), _mFragmentConf(fragmentConf)
	{
		init();
	}

	ShaderManager::~ShaderManager()
	{
		UnloadAll();
	}

	void ShaderManager::init()
	{
		readConfFile(_mVertexFolder, _mVertexConf);
		readConfFile(_mFragmentFolder, _mFragmentConf);
	}

	void ShaderManager::readConfFile(const std::string & folderPath, const std::string& confFile)
	{
		std::ifstream infile(std::string(folderPath).append(confFile));
		assert(infile.is_open() && "Shader config file not found");

		std::string line;
		while (std::getline(infile, line))
		{
			std::vector<std::string> tokenConfig;
			StringFunctions::StringFunctions::splitString(StringFunctions::StringFunctions::trim(line), ShaderManagerConstants::SEPARATOR, tokenConfig);

			assert(_mShadersDirs.find(tokenConfig[0]) == _mShadersDirs.end() && "Shaders keys are not unique");

			_mShadersDirs.insert(std::pair<std::string, std::string>(tokenConfig[0], std::string(folderPath).append(tokenConfig[1])));
		}
	}
}
