#include <SDL/SDL.h>


#ifndef BASE_H
#define BASE_H

class baseclass{
	protected:
	bool collision(SDL_Rect *rec1, SDL_Rect *rec2);//Función para la detección de collisiones, la ponemos aquí pq la vamos a usar en varias clases
	int binaryCollisionUp(SDL_Rect* rect1, SDL_Rect* rect2 ,int yvel);//Funciones para colisiones binarias
	int binaryCollisionDown(SDL_Rect* rect1,SDL_Rect* rect2,int yvel);
	int binaryCollisionRight(SDL_Rect* rect1,SDL_Rect* rect2,int xvel);
	int binaryCollisionLeft(SDL_Rect* rect1,SDL_Rect* rect2,int xvel);	
	
	public:
	static SDL_Rect coord;//Estas son las coordenadas del jugador (creo)
	static const int TILE_SIZE=50;//Esta es la medida de un cuadro para el programa de ejemplo, puede que sea necesario modificarlo
	
};

#endif


