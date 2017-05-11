#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "base.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H
class obstacle:public baseclass{//Heredamos de baseclass
	SDL_Rect box;//La caja en la que se encuentra el obstaculo
	SDL_Surface* image;
	int rotation;
	//SDL_Rect clips[]; //Esto es por si queremos darle una animacion al obstaculo como fuego o algo asi de momento usare un pincho como imagen
	public:
	obstacle(SDL_Surface* img,int x,int y,int rot);//Constructor
	SDL_Rect* getRect();
	int getRot();
	void show(SDL_Surface* screen);//mostramos por pantalla
};
#endif
