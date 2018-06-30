#include "Shader.h"

namespace ShaderFiles
{

	Shader::Shader(const std::string& filepath) : _mContent(nullptr)
	{
		LoadShader(filepath);
	}

	Shader::~Shader()
	{
		delete _mContent;
	}

	void Shader::LoadShader(const std::string & filepath)
	{
		std::ifstream infile(filepath);
		assert(infile.is_open() && "Shader file not found");

		_mContent = new std::string((std::istreambuf_iterator<char>(infile)),
			std::istreambuf_iterator<char>());
	}
}
