#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"


#ifndef FALLINGCUBE_H
#define FALLINGCUBE_H
class fallingCube:public baseclass{
	SDL_Rect box;
	int Yvel;
	bool falling;
	SDL_Surface* image;
	public:
	fallingCube(SDL_Surface* img,int x,int y,int yvel);//Constructor
	void move();
	void setFalling(bool b);
	bool getFalling();
	void show(SDL_Surface* screen);//mostramos por pantalla
	SDL_Rect* getRect();
	int getYvel();
};
#endif
