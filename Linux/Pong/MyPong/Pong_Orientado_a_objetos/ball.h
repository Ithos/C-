#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#ifndef BALL_H
#define BALL_H

class ball{
	//Aquí declaramos variables y metodos privados.
	int xvel,yvel;
	SDL_Surface* image;
	SDL_Rect box;
	bool collision(SDL_Rect* rect1,SDL_Rect* rect2);
	public://A partir de aquí declaramos los metodos públicos.
	ball(SDL_Surface* img,int x,int y,int w,int h,int xVel,int yVel);//Este es nuestro constructor.
	~ball();//En c++ necesitamos un destructor, en este caso el desructor borrará image.
	void show();
	void move(SDL_Rect* player1,SDL_Rect* player2,Mix_Chunk* ef);
	void reSet(int x,int y,int w,int h,int xVel,int yVel,Mix_Chunk* ef);
	int isOut();
};


#endif
