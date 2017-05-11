#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>

//Una skybox es un cubo enorme en el que ponemos una textura que representara el cielo y a la que no aplicaremos los desplazamientos de la camara

#ifndef SKYBOX_H
#define SKYBOX_H

class skybox{
	enum{SKY_LEFT=0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM};//Con enum damos valor al primer elemento y todos se declaran como constantes cuyo valor es el del anterior +1
	unsigned int sk[6];//Aqui almacenaremos los identificadores de las texturas
	float height;
	public:
	skybox();
	skybox(unsigned int left,unsigned int back,unsigned int rigth,unsigned int front,unsigned int top,unsigned int bottom,float s);
	~skybox();
	void drawSkybox();
	void initSkybox();
	unsigned int loadTexture(const char*);
};

#endif
