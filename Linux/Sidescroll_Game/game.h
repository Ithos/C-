#include <fstream>//Esto es para leer de archivos
#include <iostream>
#include <vector>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "base.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "menu.h"
#include "obstacle.h"
#include "movingCube.h"
#include "fallingCube.h"
#include "iceCube.h"
#include "mossCube.h"
#include "cannon.h"
#include "enemyBullet.h"
#include "turret.h"
#include "smallBullet.h"
#include "mine.h"
#include "rubberCube.h"

#ifndef GAME_H
#define GAME_H
class game:public baseclass{
	
	SDL_Surface* screen,*background,*block,*pl,*bul,*ene,*simbol,*obs,*can,*eBul,*turr,*sBul,*min;//
	SDL_Rect camera;//Creamos la camara, vamos a necesitar un rect que se mueva por el fondo y otro que se mueva por las coordenadas reales.
	std::vector<std::vector<int> > map;//Es un vector de vectores de ints, en definitiva un vector multidimensional o una matriz
	std::vector<bullet*> bullets;//Aqui almacenamos las posiciones de las balas
	std::vector<enemy*> enemies;
	std::vector<cannon*> cannons;
	std::vector<enemyBullet*> enemyBullets;
	std::vector<turret*> turrets;
	std::vector<smallBullet*> smallBullets;
	std::vector<mine*> mines;//
	std::vector<obstacle*> obstacles;
	std::vector<movingCube*> movCubes;
	std::vector<fallingCube*> fallingCubes;
	std::vector<iceCube*> iceCubes;
	std::vector<mossCube*> mossCubes;
	std::vector<rubberCube*> rubberCubes;
	bool direction[4],keyPressed;//indica la direccion de movimiento
	bool ctr,first,lvClear;
	float playerVel;
	SDL_Surface* load_image(const char* filename);
	void load_map(const char* filename);
	void showmap();
	void handleEvents();
	static const int SCREEN_WIDTH=640;//Medidas de la pantalla
	static const int SCREEN_HEIGHT=480;
	static const int FPS=30;
	int mapHeight;//Para almacenar la altura del mapa
	bool run;
	player* player1;
	SDL_Rect finish;
	
	
	public:
	void setMen(bool b);
	game();
	~game();
	bool start();
	
};
#endif
