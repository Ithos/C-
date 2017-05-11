#include<SDL/SDL.h>

int main (int argc, char** argv){

	bool run=true;	

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen;//SDL_Surface es un struct 
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption( "Hello_SDL", "Hello_SDL" );
	while(run){
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
		}
		SDL_Flip(screen);//Hace que lo que escribamos se vea en la pantalla
	}
	SDL_Quit();
	return 0;
}
