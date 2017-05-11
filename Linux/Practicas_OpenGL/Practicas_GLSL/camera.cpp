#include "camera.h"

float camX=0.0,camY=0.0,camZ=5.0;//Coordenadas de la camara
float camYaw=0.0;//Angulos de la camara
float camPitch=0.0;


#ifndef M_PI
#define M_PI 3.1415926535
#endif

void lockCamera(){
	if(camPitch>90)camPitch=90;
	if(camPitch<-90)camPitch=-90;
	if(camYaw<=0)camYaw+=360;
	if(camYaw>=360)camYaw-=360;
}

void moveCamera(float dist,float dir){
	
	float rad=(camYaw + dir)*M_PI/180;//Convertimos el angulo a radianes
	camX-=sin(rad)*dist;
	camZ-=cos(rad)*dist;
}

void moveCameraUp(float dist,float dir){
	
	float rad=(camPitch + dir)*M_PI/180;//Convertimos el angulo a radianes
	camY+=sin(rad)*dist;
}

void control(float movevel,float mousevel,bool mouseIn){
	
	if(mouseIn){//Si el raton esta en la pantalla lo bloqueamos y habilitamos movimientos de la camara
		int MidX=320;
		int MidY=240;
		SDL_ShowCursor(SDL_DISABLE);//Deshabilitamos el cursor
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);//almacenamos la posicion del raton en tmpx y tmpy
		camYaw+=mousevel*(MidX-tmpx);
		camPitch+=mousevel*(MidY-tmpy);
		lockCamera();
		SDL_WarpMouse(MidX,MidY);//Esto lleva el cursor al centro de la pantalla
		Uint8* state=SDL_GetKeyState(NULL);//Almacenamos el estado del teclado
		if(state[SDLK_w]){//Nos movemos en la dir actual
			//if(camPitch!=90 && camPitch!=-90)//Para que la camara se comporte como la de un FPS hay que comentar esta linea
				moveCamera(movevel,0.0);
			//moveCameraUp(movevel,0.0);//Para que la camara se comporte como la de un FPS hay que comentar esta linea
		}else if(state[SDLK_s]){//Nos movemos en la dir contraria
			//if(camPitch!=90 && camPitch!=-90)//Para que la camara se comporte como la de un FPS hay que comentar esta linea
				moveCamera(movevel,180.0);
			//moveCameraUp(movevel,180.0);//Para que la camara se comporte como la de un FPS hay que comentar esta linea
		}
		if(state[SDLK_a]){//izq
			moveCamera(movevel,90.0);
		}else if(state[SDLK_d]){//der
			moveCamera(movevel,270.0);
		}
	}
	//Rotamos la camara, este el raton dentro de la pantalla o no
	glRotatef(-camPitch,1.0,0.0,0.0);
	glRotatef(-camYaw,0.0,1.0,0.0);
}

void updateCamera(){//Movemos la camara
	glTranslatef(-camX,-camY,-camZ);
}

void moveTo(coordinate c){
	camX=c.x;
	camY=c.y;
	camZ=c.z;
}

coordinate camPos(){
	return coordinate(camX,camY,camZ);
}
