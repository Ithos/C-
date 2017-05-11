#ifndef TURRET_H
#define TURRET_H

#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"

class turret:public baseclass{//Heredamos de baseclass
	SDL_Rect box;//La caja en la que se encuentra el objeto
	SDL_Surface* image;
	int counter,hitCounter;//Esto lo usaremos para controlar el intervalo entre disparos y el tiempo que dura la animación de hit
	int life;
	bool hitFlag;
	double frame;
	SDL_Rect clips[10]; //Esto es por si queremos darle una animacion //de momento lo usare para la animación de hit
	public:
	turret(SDL_Surface* img,int x,int y);//Constructor
	SDL_Rect* getRect();
	void show(SDL_Surface* screen);//mostramos por pantalla
	char count(int playerPos);
	void hitCount(int playerPos);
	bool hit();
	bool getHit();
};

#endif
