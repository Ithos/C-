#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"

#ifndef ICECUBE_H
#define ICECUBE_H
class iceCube:public baseclass{
	SDL_Rect box;
	SDL_Surface* image;
	double frame;
	public:
	iceCube(SDL_Surface* img,int x,int y);//Constructor
	void actualizeFrame();
	void show(SDL_Surface* screen);//mostramos por pantalla
	SDL_Rect* getRect();
};
#endif
