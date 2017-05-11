#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>//Contiene funciones para trabajar con rangos de elementos
#include <fstream>
#include <iostream>
#include <cstdio>
#include "objLoader.h"

#ifndef ANIMLOADER_H
#define ANIMLOADER_H

void loadAnimation(std::vector<unsigned int>& frames,std::string filename,unsigned int num,objLoader &loader);

#endif
