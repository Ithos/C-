#include "cannon.h"

cannon::cannon(SDL_Surface* img,int x,int y,bool b){
	box.x=x;
	box.y=y;
	box.h=box.w=32;
	up=b;
	if(b)image=rotozoomSurface(img,180,1.0,0);
	else image=img;
	counter=0;
	hitCounter=0;
	life=4;
	hitFlag=false;
	if(up)frame=1.0;
	else frame=0.0;
	for(int i=0;i<2;i++){
		clips[i].x=i*32;
		clips[i].y=0;
		clips[i].h=32;
		clips[i].w=32;
	}
}

void cannon::show(SDL_Surface* screen){
	SDL_Rect tmp={box.x-coord.x,box.y-coord.y,32,32};//Convertimos a coordenadas relativas a la pantalla
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&tmp);//Bliteamos
}

SDL_Rect* cannon::getRect(){
	return &box;
}

bool cannon::getUpDown(){
	return up;
}

bool cannon::count(){
	counter++;
	if(counter>60){
		counter=0;
		return true;
	}
	else return false;
}

void cannon::hitCount(){
	hitCounter++;
	if(hitCounter>15){
		hitCounter=0;
		if(up)frame=1.0;
		else frame=0.0;
		hitFlag=false;
		return;
	}
	if(hitCounter%4==0){
		if(frame==0.0)frame=1.0;
		else frame=0.0;
	}
	
}

bool cannon::hit(){
	if(!hitFlag){
		life--;
		hitFlag=true;
	}
	if(life<=0)return true;
	else return false;
}

bool cannon::getHit(){
	return hitFlag;
}
