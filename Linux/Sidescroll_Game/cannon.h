#ifndef CANNON_H
#define CANNON_H

#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h> //Podria ser interesante si queremos hacer una animación que siga al jugador
#include "base.h"

class cannon:public baseclass{//Heredamos de baseclass
	SDL_Rect box;//La caja en la que se encuentra el obstaculo
	SDL_Surface* image;
	int counter,hitCounter;//Esto lo usaremos para controlar el intervalo entre disparos y el tiempo que dura la animación de hit
	int life;
	bool up,hitFlag;
	double frame;
	SDL_Rect clips[2]; //Esto es por si queremos darle una animacion //de momento lo usare para la animación de hit
	public:
	cannon(SDL_Surface* img,int x,int y,bool b);//Constructor
	SDL_Rect* getRect();
	bool getUpDown();
	void show(SDL_Surface* screen);//mostramos por pantalla
	bool count();
	void hitCount();
	bool hit();
	bool getHit();
};


#endif
