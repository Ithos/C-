#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"
#include "movingCube.h"

#ifndef PLAYER_H
#define PLAYER_H

class player:baseclass{
	SDL_Rect box;//Aqui guardamos las coordenadas del jugador relativas a la pantalla
	SDL_Surface *image;//Aqui cargamos la imagen del personaje
	int Xvel,Yvel;//Velocidades
	int Xvel_Abs,Yvel_Abs;//Son velocidades en coordenadas absolutas, de momento no uso la y pero la x es necesaria para las colisiones
	SDL_Rect clips[18];//Rects para las distintas posiciones de la imagen, es posible que haya que cambiar el numero
	bool ground,jump,fastFall,cubeFlag,mossy,onIce,onAir;
	int nJump;
	char direction;
	bool moving;
	double frame;
	int health;//El medidor de salud
	bool Xcollision;
	public:
	player(SDL_Surface *img);//En el constructor básicamente cargamos la imagen
	~player();//En el destructor liberamos el surface de la imagen
	SDL_Rect *getRect();//Sets y gets
	void setXvel(int vel);
	int getXvel();
	void setXvel_Abs(int vel);
	int getXvel_Abs();
	void setYvel(int vel);
	int getYvel();
	void setYvel_Abs(int vel);
	int getYvel_Abs();
	void move(const std::vector<std::vector<int> >& map);//Esta funcion es para chequear el mapa, creo
	void show(SDL_Surface *screen);
	void setJump(int strenght=15);//Esta funcion es para hacer los saltos
	void stopJump();//Esta funcion es para parar un salto
	void setMoving(bool b);
	void setDirection(char c);
	char getDirection();
	void setHealth(int h);
	int getHealth();
	void setX(int x);
	void setY(int y);
	bool getGround();
	void setGroundTrue();
	void setCFlag(bool b);
	void setSecondJump();
	bool getMossy();
	void setMossy(bool moss);
	void setOnIce(bool ice);
	bool getOnIce();
	bool getXcol();
	
};

#endif
