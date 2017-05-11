#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#ifndef MENU_H
#define MENU_H
class menu{
	static const int NUMENU=2;
	SDL_Surface* menus[NUMENU];
	SDL_Rect pos[NUMENU];
	TTF_Font* font;
	Mix_Chunk* effect;
	public:
	menu();
	~menu();//Aqui cerramos menus
	int show(SDL_Surface* screen,const char* c);
	void showmessage(SDL_Surface* screen,const char* c,int r,int g, int b);//Para mostrar mensajes por pantalla


};
#endif
