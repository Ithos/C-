#include "turret.h"

turret::turret(SDL_Surface* img,int x,int y){
	box.x=x;
	box.y=y;
	box.h=30;
	box.w=35;
	image=img;
	counter=0;
	hitCounter=0;
	life=3;
	hitFlag=false;
	frame=0.0;
	for(int i=0;i<10;i++){
		clips[i].x=i*35;
		clips[i].y=0;
		clips[i].h=30;
		clips[i].w=35;
	}
}

void turret::show(SDL_Surface* screen){
	SDL_Rect tmp={box.x-coord.x,box.y-coord.y,35,30};//Convertimos a coordenadas relativas a la pantalla
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&tmp);//Bliteamos
}

SDL_Rect* turret::getRect(){
	return &box;
}

char turret::count(int playerPos){
	counter++;
	
	if((frame<3.5||(frame>4.5&&frame<8.5))&&playerPos+coord.x>box.x+box.w*3/2)frame+=0.3;
	if(((frame>0.5&&frame<4.5)||frame>5.5)&&playerPos+coord.x<box.x-box.w*0.5)frame-=0.3;
	
	if(counter>40){
		counter=0;
		if((frame>-0.6&&frame<0.5)||(frame>4.4&&frame<5.5))return 'l';
		else if((frame>3.4&&frame<4.5)||(frame>8.4&&frame<9.5))return 'r';
		else return 'm';
	}
	else return 'm';
}

void turret::hitCount(int playerPos){
	hitCounter++;
	if(hitCounter>15){
		hitCounter=0;
		if(frame>4.4){
			frame-=5.0;
			if(frame<0.0)frame=0.0;
		}
		hitFlag=false;
		return;
	}
	if(hitCounter%4==0){
		if(frame<=4.4){
			frame+=5.0;
			if(frame>9.4)frame=9.4;
		}else{
			 frame-=5.0;
			 if(frame<0.0)frame=0.0;
		 }
	}
	
}

bool turret::hit(){
	if(!hitFlag){
		life--;
		hitFlag=true;
	}
	if(life<=0)return true;
	else return false;
}

bool turret::getHit(){
	return hitFlag;
}
