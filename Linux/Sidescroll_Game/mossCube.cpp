#include "mossCube.h"

mossCube::mossCube(SDL_Surface* img,int x,int y,bool rot){
	image=img;//Almacenamos las distintas variables que nos pasan
	box.x=x;
	box.y=y;
	box.w=box.h=baseclass::TILE_SIZE;
	rotation=rot; 
}

void mossCube::show(SDL_Surface* screen){
	int n=11;
	if (rotation)n=12;
	SDL_Rect tmp={box.x-baseclass::coord.x,box.y-baseclass::coord.y,box.w,box.h};//Creamos un rect cuyas coordenadas son relativas a la pantalla
	SDL_Rect block={n*baseclass::TILE_SIZE,0,baseclass::TILE_SIZE,baseclass::TILE_SIZE};//Movemos el recuadro que representamos en la imagen de bloques
	SDL_BlitSurface(image,&block,screen,&tmp);//Bliteamos con el tmp que hemos creado
}

SDL_Rect* mossCube::getRect(){
	return &box;
}

bool mossCube::getRotation(){
	return rotation;
}
