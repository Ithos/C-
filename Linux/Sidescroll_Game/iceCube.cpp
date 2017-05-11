#include "iceCube.h"

iceCube::iceCube(SDL_Surface* img,int x,int y){
	frame=-10.0;
	image=img;//Almacenamos las distintas variables que nos pasan
	box.x=x;
	box.y=y;
	box.w=box.h=baseclass::TILE_SIZE; 
}

void iceCube::actualizeFrame(){
	frame+=0.2;
	if(frame>5)frame=-10.0;
}

void iceCube::show(SDL_Surface* screen){
	int n=(int)(frame+0.5);
	if(n<2)n=2;
	SDL_Rect tmp={box.x-baseclass::coord.x,box.y-baseclass::coord.y,box.w,box.h};//Creamos un rect cuyas coordenadas son relativas a la pantalla
	SDL_Rect block={n*baseclass::TILE_SIZE,0,baseclass::TILE_SIZE,baseclass::TILE_SIZE};//Movemos el recuadro que representamos en la imagen de bloques
	SDL_BlitSurface(image,&block,screen,&tmp);//Bliteamos con el tmp que hemos creado
}

SDL_Rect* iceCube::getRect(){
	return &box;
}

