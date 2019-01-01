#pragma once

#ifndef __SHADERFILES_SHADER_H
#define __SHADERFILES_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

namespace ShaderFiles
{
	class Shader
	{
	public:
		Shader(const std::string& filepath);
		virtual ~Shader();
		std::string& getContent() { return (*_mContent); }

	private:
		std::string* _mContent;
		void LoadShader(const std::string& filepath);

	};
}
#endif