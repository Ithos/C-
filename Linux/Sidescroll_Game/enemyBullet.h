#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H
#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"
#include "obstacle.h"
#include "movingCube.h"
#include "fallingCube.h"

class enemyBullet:baseclass{
	SDL_Rect box;
	int Xvel,Yvel;
	SDL_Surface *image;
	SDL_Rect clips[3];
	double frame;
	
	public:
	enemyBullet(SDL_Surface *img,int x,int y,int xvel,int yvel);//No incluimos destructor porque liberamos image cada vez que la usamos// 
	void show(SDL_Surface *screen); 
	void move(int playerX, int playerY);
	SDL_Rect* getRect();
	bool bulletCollision(const std::vector<std::vector<int> >& map);
	int getXvel();
	int getYvel();
	void setXvel(int vel);
	void setYvel(int vel);
	bool obstacleCollision(const std::vector<obstacle*>& obstacles);//Colisiones entre balas y obstaculos si sucede true, si no false
	bool movCubeCollision(const std::vector<movingCube*>& obstacles);//Colisiones entre balas y cubos moviles
	bool fallCubeCollision(const std::vector<fallingCube*>& obstacles);//Colisiones entre balas y cubos que se caen
};


#endif
