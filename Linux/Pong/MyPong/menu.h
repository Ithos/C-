#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#ifndef MENU_H
#define MENU_H
class menu{
	static const int NUMENU=2;
	SDL_Surface* menus[NUMENU];
	SDL_Rect pos[NUMENU];
	public:
	menu();
	~menu();//Aqui cerramos menus
	int show(SDL_Surface* screen,TTF_Font* font);


};
#endif
