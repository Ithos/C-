#include "cameraClass.h"

void camera::lockCamera(){
	if(camPitch>90)camPitch=90;
	if(camPitch<-90)camPitch=-90;
	if(camYaw<=0)camYaw+=360;
	if(camYaw>=360)camYaw-=360;
}

void camera::moveCamera(float dir){
	float rad=(camYaw + dir)*M_PI/180;//Convertimos el angulo a radianes
	loc.x-=sin(rad)*movevel;
	loc.z-=cos(rad)*movevel;
}

void camera::moveCameraUp(float dir){
	float rad=(camPitch + dir)*M_PI/180;//Convertimos el angulo a radianes
	loc.y+=sin(rad)*movevel;
}

camera::camera(){
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=false;
}

camera::camera(vector3d l){
	loc.change(l);
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=false;
}

camera::camera(vector3d l,float yaw,float pitch){
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=0.2;
	mousevel=0.2;
	mi=false;
}

camera::camera(vector3d l,float yaw,float pitch,float mv,float mov){
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=mv;
	mousevel=mov;
	mi=false;
}

void camera::control(){
	if(mi){//Si el raton esta en la pantalla lo bloqueamos y habilitamos movimientos de la camara
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
				moveCamera(0.0);
			//moveCameraUp(movevel,0.0);//Para que la camara se comporte como la de un FPS hay que comentar esta linea
		}else if(state[SDLK_s]){//Nos movemos en la dir contraria
			//if(camPitch!=90 && camPitch!=-90)//Para que la camara se comporte como la de un FPS hay que comentar esta linea
				moveCamera(180.0);
			//moveCameraUp(movevel,180.0);//Para que la camara se comporte como la de un FPS hay que comentar esta linea
		}
		if(state[SDLK_a]){//izq
			moveCamera(90.0);
		}else if(state[SDLK_d]){//der
			moveCamera(270.0);
		}
	}
	//Rotamos la camara, este el raton dentro de la pantalla o no
	glRotatef(-camPitch,1.0,0.0,0.0);
	glRotatef(-camYaw,0.0,1.0,0.0);
}

void camera::updateCamera(){
	glTranslatef(-loc.x,-loc.y,-loc.z);
}

vector3d camera::getVector(){
	//Damos el vector en coordenadas cartesianas segun el cual está mirando la cámara, para ello tenemos que convertir este vector de coordenadas esféricas a cartesianas
	return (vector3d(-cos(camPitch*M_PI/180)*sin(camYaw*M_PI/180),sin(camYaw*M_PI/180),-cos(camPitch*M_PI/180)*cos(camYaw*M_PI/180)));
}

vector3d camera::getLocation(){
	return loc;
}

float camera::getPitch(){
	return camPitch;
}

float camera::getYaw(){
	return camYaw;
}

float camera::getMovevel(){
	return movevel;
}

float camera::getMousevel(){
	return mousevel;
}

bool camera::isMouseIn(){
	return mi;
}

void camera::setLocation(vector3d l){
	loc.change(l);
}

void camera::lookAt(float pitch,float yaw){
	camPitch=pitch;
	camYaw=yaw;
}

void camera::mouseIn(bool b){
	mi=b;
}

void camera::setSpeed(float mv,float mov){
	movevel=mv;
	mousevel=mov;
}
