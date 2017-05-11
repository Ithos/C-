#include "modifiedCameraClass2.h"

void camera::lockCamera(){
	if(camPitch>90)camPitch=90;
	if(camPitch<-90)camPitch=-90;
	if(camYaw<=0)camYaw+=360;
	if(camYaw>=360)camYaw-=360;
}

void camera::moveCamera(float dir){
	float rad=(camYaw + dir)*M_PI/180;//Convertimos el angulo a radianes
	loc.setX(loc.getX()-sin(rad)*movevel);
	loc.setZ(loc.getZ()-cos(rad)*movevel);
}

void camera::moveCameraUp(float dir){
	float rad=(camPitch + dir)*M_PI/180;//Convertimos el angulo a radianes
	loc.setY(loc.getY()+sin(rad)*movevel);
}

camera::camera(bool f){
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=false;
	free=f;
}

camera::camera(vector3d l,bool f){
	loc.change(l);
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=false;
	free=f;
}

camera::camera(vector3d l,float yaw,float pitch,bool f){
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=0.2;
	mousevel=0.2;
	mi=false;
	free=f;
}

camera::camera(vector3d l,float yaw,float pitch,float mv,float mov,bool f){
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=mv;
	mousevel=mov;
	mi=false;
	free=f;
}

void camera::control(matrices &pipeline){
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
			if((camPitch!=90 && camPitch!=-90) || !free)//si free es false la cámara se comporta como la de un FPS
				moveCamera(0.0);
			if(free)moveCameraUp(0.0);//si free es false la cámara se comporta como la de un FPS
		}else if(state[SDLK_s]){//Nos movemos en la dir contraria
			if((camPitch!=90 && camPitch!=-90) || !free)//si free es false la cámara se comporta como la de un FPS
				moveCamera(180.0);
			if(free)moveCameraUp(180.0);//si free es false la cámara se comporta como la de un FPS
		}
		if(state[SDLK_a]){//izq
			moveCamera(90.0);
		}else if(state[SDLK_d]){//der
			moveCamera(270.0);
		}
	}
	//Rotamos la camara, este el raton dentro de la pantalla o no
	pipeline.matrixMode(VIEW_MATRIX);
	pipeline.rotateX(camPitch);//No damos signo - porque se supone que empleamos el modo VIEW_MATRIX, que ya lo incluye
	pipeline.rotateY(camYaw);
	pipeline.matrixMode(MODEL_MATRIX);
}

void camera::updateCamera(matrices &pipeline){
	pipeline.matrixMode(VIEW_MATRIX);
	pipeline.translate(loc.getX(),loc.getY(),loc.getZ()); //Lo mismo con el signo - en translate
	pipeline.matrixMode(MODEL_MATRIX);
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
