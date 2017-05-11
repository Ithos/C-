#include "ball.h"

ball::ball(SDL_Surface* img,int x,int y,int w,int h,int xVel,int yVel){//Aquí implementamos el constructor



	image=img;
	box.x=x;
	box.y=y;
	box.w=w;
	box.h=h;
	xvel=xVel;
	yvel=yVel;


}

ball::~ball(){//Implementamos el destructor
	SDL_FreeSurface(image);
}

//A partir de aquí implementamos los metodos que hemos declarado

void ball::show(){
	SDL_BlitSurface(image,NULL,SDL_GetVideoSurface(),&box);
}

void ball::move(SDL_Rect* player1,SDL_Rect* player2,Mix_Chunk* ef){
	box.x+=xvel;
	box.y+=yvel;

	if(box.y>0)yvel=-yvel;
	if(box.y+box.h<SDL_GetVideoSurface()->clip_rect.h)yvel=-yvel;

	if(collision(&box,player1)){
		Mix_PlayChannel(0,ef,1);
		if(box.x<player2->x+player1->w){
			yvel=-yvel;
		}else
		xvel=-xvel;
	}

	if(collision(&box,player2)){
		Mix_PlayChannel(0,ef,1);
		if(box.x+box.w>player1->x){
			yvel=-yvel;
		}else		
		xvel=-xvel;
	}
	
}

void ball::reSet(int x,int y,int w,int h,int xVel,int yVel,Mix_Chunk* ef){
	box.x=x;
	box.y=y;
	box.w=w;
	box.h=h;
	xvel=xVel;
	yvel=yVel;
	Mix_PlayChannel(0,ef,1);
	
}

int ball::isOut(){
	if(box.x<=0)return 1;
	if(box.x>=SDL_GetVideoSurface()->clip_rect.w)return 2;
	return 0;
}

bool ball::collision(SDL_Rect* rect1, SDL_Rect* rect2){//Chequeamos cuando no hay colisión y devolvemos 0, si no devolvemos 1
	if(rect1->y >= rect2->y + rect2->h)return 0;
	if(rect1->x >= rect2->x + rect2->w)return 0;
	if(rect1->y + rect1->h <= rect2->y)return 0;
	if(rect1->x + rect1->w <= rect2->x)return 0;
	return 1;		
}
