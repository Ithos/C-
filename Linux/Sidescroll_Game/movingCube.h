#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"

#ifndef MOVINGCUBE_H
#define MOVINGCUBE_H
class movingCube:public baseclass{
	SDL_Rect box;
	int Xvel,Yvel;
	int d,Xini,Yini;
	SDL_Surface* image;
	bool horizontal;//Esto nos dice si la distancia indicada es en horizontal o en vertical
	public:
	movingCube(SDL_Surface* img,int x,int y,int xvel,int yvel,int dist,bool hor);//constructor
	void move();//Movemos sin tener en cuenta colisiones para dar mas versatilidad 
	void refresh();//Actualizamos las velocidades de forma independiente del movimiento
	void show(SDL_Surface* screen);//mostramos por pantalla
	SDL_Rect* getRect();
	int getXvel();
	int getYvel();
};
#endif
