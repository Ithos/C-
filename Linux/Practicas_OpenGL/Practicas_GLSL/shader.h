#ifndef SHADER_H
#define SHADER_H
#include "GLee.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class shader{
	unsigned int vs,fs,program;
	unsigned int loadShader(std::string &source,unsigned int mode);
	void loadFile(const char* fn,std::string &str);
	public:
	shader(const char *vss,const char *fss);
	~shader();
	void useShader();
	void delShader();
	unsigned int getProgramId();
};

#endif
