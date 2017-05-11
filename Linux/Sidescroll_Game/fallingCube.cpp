#include "fallingCube.h"


fallingCube::fallingCube(SDL_Surface* img,int x,int y,int yvel){
	image=img;//Almacenamos las distintas variables que nos pasan
	box.x=x;
	box.y=y;
	box.w=box.h=baseclass::TILE_SIZE;
	Yvel=yvel;
	falling=false;//Ponemos falling a false 
}

void fallingCube::show(SDL_Surface* screen){
	SDL_Rect tmp={box.x-baseclass::coord.x,box.y-baseclass::coord.y,box.w,box.h};//Creamos un rect cuyas coordenadas son relativas a la pantalla
	SDL_Rect block={7*baseclass::TILE_SIZE,0,baseclass::TILE_SIZE,baseclass::TILE_SIZE};//Movemos el recuadro que representamos en la imagen de bloques
	SDL_BlitSurface(image,&block,screen,&tmp);//Bliteamos con el tmp que hemos creado
}

SDL_Rect* fallingCube::getRect(){
	return &box;
}

int fallingCube::getYvel(){
	return Yvel;
}

bool fallingCube::getFalling(){
	return falling;
}

void fallingCube::setFalling(bool b){
	falling=b;
}

void fallingCube::move(){
	if(falling)box.y+=Yvel;//Si tanamos falling a true movemos el cubo
}
