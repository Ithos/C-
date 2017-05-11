#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"
#include "obstacle.h"

#ifndef ENEMY_H
#define ENEMY_H
class enemy:public baseclass{
		SDL_Rect box;
		int Xvel,Yvel;
		//int d,Xini,Yini;//Esto seria para hacer que no tuvieran que chocarse para cambiar de direccion
		SDL_Surface* image;
		bool ground;
		bool jump;
		bool stomped,hit,del;
		int counter;
		SDL_Rect clips[11];
		double frame;
		char direction;
		public:
		enemy(SDL_Surface* img,int x,int y,int xvel,int yvel);//constructor//int dist
		void move(const std::vector<std::vector<int> >& map);//pasamos el vector map
		void show(SDL_Surface* screen);//mostramos por pantalla
		SDL_Rect* getRect();
		void enemyCollision();//Esto es para cuando el enemigo choca con algo distinto del mapa que se gestiona en game
		void obstacleCollision(const std::vector<obstacle*>& obstacles);//Aqui manejamos las colisiones con obstaculos
		void setHit();
		bool getHit();
		void setStomp();
		bool getStomp();
		bool getDeletion();
		void increaseTimeCounter();
	
};
#endif
