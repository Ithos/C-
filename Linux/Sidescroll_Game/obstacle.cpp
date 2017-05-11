#include "obstacle.h"

obstacle::obstacle(SDL_Surface* img, int x, int y,int rot){
	if (rot<0||rot>3)rot=0;
	if(rot==0){
		image=img;
	}else if(rot==1){
		image=rotozoomSurface(img,180,1.0,0);
	}else if(rot==2){
		image=rotozoomSurface(img,90,1.0,0);
	}else if(rot==3){
		image=rotozoomSurface(img,-90,1.0,0);
	}
	box.x=x;
	box.y=y;
	box.h=box.w=40;
	rotation=rot;
}

void obstacle::show(SDL_Surface* screen){
	SDL_Rect tmp={box.x-coord.x,box.y-coord.y,40,40};//Convertimos a coordenadas relativas a la pantalla
	SDL_BlitSurface(image,NULL,screen,&tmp);//Bliteamos
}

SDL_Rect* obstacle::getRect(){
	return &box;
}
int obstacle::getRot(){
	return rotation;
}


