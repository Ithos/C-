#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <vector>
#include "base.h"

#ifndef RUBBERCUBE_H
#define RUBBERCUBE_H
class rubberCube:public baseclass{
	SDL_Rect box;
	SDL_Surface* image;
	double frame;
	public:
	rubberCube(SDL_Surface* img,int x,int y);//Constructor
	void show(SDL_Surface* screen);//mostramos por pantalla
	SDL_Rect* getRect();
};
#endif
