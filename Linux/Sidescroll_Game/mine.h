#ifndef MINE_H
#define MINE_H
#include <stdio.h>     
#include <stdlib.h>  
#include <time.h> 
#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"
#include "obstacle.h"
#include "movingCube.h"
#include "fallingCube.h"

class mine:public baseclass{//Heredamos de baseclass
	SDL_Rect box;//La caja en la que se encuentra el objeto
	int velx,vely;
	SDL_Surface* image;
	double frame;
	bool hitFlag;
	SDL_Rect clips[5];
	bool mapCollision(const std::vector<std::vector<int> >& map);
	bool obstacleCollision(const std::vector<obstacle*>& obstacles);
	bool movCubeCollision(const std::vector<movingCube*>& obstacles);
	bool fallCubeCollision(const std::vector<fallingCube*>& obstacles);
	public:
	mine(SDL_Surface* img,int x,int y);
	SDL_Rect* getRect();
	void show(SDL_Surface* screen);//mostramos por pantalla
	bool move(int playerX,int playerY,const std::vector<std::vector<int> >& map,std::vector<obstacle*> obstacles,std::vector<movingCube*> movCubes,std::vector<fallingCube*> fallCubes);
	void hit();
	bool getHit();
};

#endif
