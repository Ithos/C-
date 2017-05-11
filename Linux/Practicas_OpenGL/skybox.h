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

void drawSkybox(float);
void initSkybox();
void killSkybox();
unsigned int loadTexture(const char*);

#endif
