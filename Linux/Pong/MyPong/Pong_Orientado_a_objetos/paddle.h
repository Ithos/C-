#include <SDL/SDL.h>
#ifndef PADDLE_H
#define PADDLE_H

class paddle{
	//Aquí declaramos variables y metodos privados.
	int yvel;
	int points;
	SDL_Surface* image;
	SDL_Rect box;
	public://A partir de aquí declaramos los metodos públicos.
	paddle(SDL_Surface* img,int x,int y,int w,int h,int yVel);//Este es nuestro constructor.
	~paddle();//En c++ necesitamos un destructor, en este caso el desructor borrará image.
	void show();
	void moveUp();
	void moveDown();
	SDL_Rect* getRect();
	void incPoint();
	int getPoints();
};


#endif
