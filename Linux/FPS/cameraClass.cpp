#include "cameraClass.h"

void camera::lockCamera(){
	if(camPitch>90)camPitch=90;
	if(camPitch<-90)camPitch=-90;
	if(camYaw<=0)camYaw+=360;
	if(camYaw>=360)camYaw-=360;
}

void camera::moveCamera(float dir,vector3d& newPos){
	float rad=(camYaw + dir)*M_PI/180;//Convertimos el angulo a radianes
	newPos.setX(newPos.getX()-sin(rad)*movevel);
	newPos.setZ(newPos.getZ()-cos(rad)*movevel);
}

void camera::moveCameraUp(float dir,vector3d& newPos){
	float rad=(camPitch + dir)*M_PI/180;//Convertimos el angulo a radianes
	newPos.setY(newPos.getY()+sin(rad)*movevel);
}

camera::camera(bool f){
	freeCam=f;
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=moving=false;
}

camera::camera(vector3d l,bool f){
	loc.change(l);
	camPitch=0;
	camYaw=0;
	movevel=0.2;
	mousevel=0.2;
	mi=moving=false;
	freeCam=f;
}

camera::camera(vector3d l,float yaw,float pitch,bool f){
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=0.2;
	mousevel=0.2;
	mi=moving=false;
	freeCam=f;
}

camera::camera(vector3d l,float yaw,float pitch,float mv,float mov,bool f){
	loc.change(l);
	camPitch=pitch;
	camYaw=yaw;
	movevel=mv;
	mousevel=mov;
	mi=moving=false;
	freeCam=f;
}

vector3d camera::control(){
	vector3d newPos;
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
		moving=false;
		if(state[SDLK_w]){//Nos movemos en la dir actual
			moving=true;
			if((camPitch!=90 && camPitch!=-90)||!freeCam)//Para que la camara se comporte como la de un FPS hay que comentar esta linea
				moveCamera(0.0,newPos);
			if(freeCam)moveCameraUp(0.0,newPos);//Para que la camara se comporte como la de un FPS hay que comentar esta linea
		}else if(state[SDLK_s]){//Nos movemos en la dir contraria
			moving=true;
			if((camPitch!=90 && camPitch!=-90)||!freeCam)//Para que la camara se comporte como la de un FPS hay que comentar esta linea
				moveCamera(180.0,newPos);
			if(freeCam)moveCameraUp(180.0,newPos);//Para que la camara se comporte como la de un FPS hay que comentar esta linea
		}
		if(state[SDLK_a]){//izq
			moving=true;
			moveCamera(90.0,newPos);
		}else if(state[SDLK_d]){//der
			moving=true;
			moveCamera(270.0,newPos);
		}
	}
	//Rotamos la camara, este el raton dentro de la pantalla o no
	glRotatef(-camPitch,1.0,0.0,0.0);
	glRotatef(-camYaw,0.0,1.0,0.0);
	return newPos;
}

void camera::updateCamera(){
	glTranslatef(-loc.getX(),-loc.getY(),-loc.getZ());
}

vector3d camera::getVector(){
	//Damos el vector en coordenadas cartesianas segun el cual está mirando la cámara, para ello tenemos que convertir este vector de coordenadas esféricas a cartesianas
	return (vector3d(-cos(camPitch*M_PI/180)*sin(camYaw*M_PI/180),sin(camPitch*M_PI/180),-cos(camPitch*M_PI/180)*cos(camYaw*M_PI/180)));
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

bool camera::getMoving(){
	return moving;
}
