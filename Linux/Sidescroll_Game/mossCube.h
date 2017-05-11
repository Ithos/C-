#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"

#ifndef MOSSCUBE_H
#define MOSSCUBE_H
class mossCube:public baseclass{
	SDL_Rect box;
	SDL_Surface* image;
	bool rotation;
	public:
	mossCube(SDL_Surface* img,int x,int y,bool rot);//Constructor
	void show(SDL_Surface* screen);//mostramos por pantalla
	bool getRotation();
	SDL_Rect* getRect();
};
#endif
