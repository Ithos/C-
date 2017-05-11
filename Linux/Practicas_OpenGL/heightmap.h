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


//Estas cosas se usan para generar terrenos
#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

void loadHeightmap(std::vector<std::vector<float> >& heights, const char* name);
void renderHeightmap(std::vector<std::vector<float> >heights,float size,float h);

#endif
