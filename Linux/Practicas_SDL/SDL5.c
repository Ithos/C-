#include<SDL/SDL.h>
#include "SDL/SDL_image.h"

int main (int argc, char** argv){

	bool run=true;
	bool key[4]={0,0,0,0};
	Uint32 start; //Integer 32 bits	
	const int FPS=30;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen, *image;//SDL_Surface es un struct
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption( "Hello_SDL", "Hello_SDL" );
	
	SDL_Rect rect1;
	rect1.x=50;
	rect1.y=40;
	rect1.w=20;
	rect1.h=20;
	Uint32 color1=SDL_MapRGB(screen->format,0xaa,0x55,0xff); //Definimos un color
	SDL_Rect rect2;
	rect2.x=250;
	rect2.y=400;
	rect2.w=60;
	rect2.h=30;
	Uint32 color2=SDL_MapRGB(screen->format,0x33,0x55,0xcc); //Definimos un color
	Uint32 color3=SDL_MapRGB(screen->format,0x33,0xaa,0x55); //Definimos un color
	image=IMG_Load("bmp.jpeg");
	SDL_Rect rect;
	rect.x=200;
	rect.y=150;
	rect.h=600;
	rect.w=300;
	
	while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_UP)key[0]=1;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=1;
				if(event.key.keysym.sym==SDLK_RIGHT)key[2]=1;
				if(event.key.keysym.sym==SDLK_LEFT)key[3]=1;
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP)key[0]=0;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=0;
				if(event.key.keysym.sym==SDLK_RIGHT)key[2]=0;
				if(event.key.keysym.sym==SDLK_LEFT)key[3]=0;
			}
		}//A partir de aquí empieza la logica del programa
		if(key[0]&&rect1.y!=0)rect1.y--;
		if(key[1]&&rect1.y!=460)rect1.y++;
		if(key[2]&&rect1.x!=620)rect1.x++;
		if(key[3]&&rect1.x!=0)rect1.x--;


		//Aqui dibujamos
		
		SDL_FillRect(screen,&screen->clip_rect,color3);//Dibuja por orden, asique en este caso sobre el fondo verde se dibuja todo
		SDL_BlitSurface(image,&rect,screen,&rect);
		SDL_FillRect(screen,&rect1,color1);//Si lo haces mal dibujas el fondo sobre otras cosas y no se ven
		SDL_FillRect(screen,&rect2,color2);
		
		
		SDL_Flip(screen);
		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
		
	}
	SDL_FreeSurface(image);
	SDL_Quit();
	return 0;
}
