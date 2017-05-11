#include "movingCube.h"

movingCube::movingCube(SDL_Surface* img,int x,int y,int xvel,int yvel,int dist,bool hor){
	box.x=Xini=x;//Establecemos la caja en la posicion inicial y almacenamos la posicion
	box.y=Yini=y;
	box.w=box.h=50;
	d=baseclass::TILE_SIZE*dist;//La distancia indicada es en bloques
	horizontal=hor;
	Xvel=xvel;
	Yvel=yvel;
	image=img;
}

void movingCube::show(SDL_Surface* screen){
	SDL_Rect tmp={box.x-baseclass::coord.x,box.y-baseclass::coord.y,box.w,box.h};//Creamos un rect cuyas coordenadas son relativas a la pantalla
	SDL_Rect block={6*baseclass::TILE_SIZE,0,baseclass::TILE_SIZE,baseclass::TILE_SIZE};//Movemos el recuadro que representamos en la imagen de bloques
	SDL_BlitSurface(image,&block,screen,&tmp);//Bliteamos con el tmp que hemos creado
}

SDL_Rect* movingCube::getRect(){
	return &box;
}

int movingCube::getXvel(){
	return Xvel;
}

int movingCube::getYvel(){
	return Yvel;
}

void movingCube::move(){
	box.x+=Xvel;
	box.y+=Yvel;
}

void movingCube::refresh(){
	if(horizontal){
		if(Xvel>0){
			if(box.x<Xini+d){//Comprobamos si hemos recorrido la distancia indicada
			}else{
				Xvel=-Xvel;//Si hemos llegado al max cambiamos las velocidades para volver
				Yvel=-Yvel;
			}
		}else{
			if(box.x>Xini){//Comprobamos si hemos recorrido la distancia indicada
			}else{
				Xvel=-Xvel;//Si hemos llegado al max cambiamos las velocidades para volver
				Yvel=-Yvel;
			}
		}
	}else if(Yvel>0){ 
		if(box.y<Yini+d){
		}else{
			Xvel=-Xvel;
			Yvel=-Yvel;
		}
	}else{
		if(box.y>Yini){
		}else{
			Xvel=-Xvel;
			Yvel=-Yvel;
		}
	}
}
