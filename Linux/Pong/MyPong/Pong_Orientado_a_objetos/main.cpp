#include<SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include<iostream>
#include"ball.h"
#include"paddle.h"
#include"menu.h"


int main(){
	
	
	bool run=true;
	bool key[4]={0,0,0,0};
	Uint32 start; //Integer 32 bits	
	const int FPS=30;
	char c[5];

	SDL_Init(SDL_INIT_EVERYTHING);

	
	
	
	SDL_Surface* screen=SDL_SetVideoMode(800,600,32,SDL_SWSURFACE);;
	SDL_WM_SetCaption( "Pong", "Pong" );
	SDL_Surface* field=IMG_Load("field2.bmp");

	SDL_Event event;

	TTF_Init();
	TTF_Font* font=	TTF_OpenFont("DroidSansMono.ttf",32);

	
	
	SDL_Surface* text,*icon=IMG_Load("ball2.bmp");
	SDL_SetColorKey(icon,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,255,255,255));

	SDL_WM_SetIcon(icon,NULL);

	SDL_Rect tmp={10,0};
	

	SDL_Color color={255,255,255};
	

	paddle player2(IMG_Load("pala1.bmp"),80,300,10,50,7);
	paddle player1(IMG_Load("pala2.bmp"),720,300,10,50,7);
	
	ball ball1(icon,400,300,10,10,3,3);

	Mix_Chunk* effect1;
	Mix_Chunk* effect2;
	Mix_Music* music;
	Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
	music=Mix_LoadMUS("Fisherman's Horizon.mp3");
	effect1=Mix_LoadWAV("efect.wav");
	effect2=Mix_LoadWAV("efect2.wav");

	Mix_PlayMusic(music,-1);
	Mix_VolumeMusic(10);//de 0 a 128

	
	
	int i=menu().show(screen,font);

	if(i==1)run=false;

	while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_UP)key[0]=1;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=1;
				if(event.key.keysym.sym==SDLK_w)key[2]=1;
				if(event.key.keysym.sym==SDLK_s)key[3]=1;
				if(event.key.keysym.sym==SDLK_ESCAPE){
					i=menu().show(screen,font);
					if(i==1)run=false;
				}
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP)key[0]=0;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=0;
				if(event.key.keysym.sym==SDLK_w)key[2]=0;
				if(event.key.keysym.sym==SDLK_s)key[3]=0;
			}
		}

	//Logica del programa

	//Movimiento de las palas
	
	if(key[0]){
		player1.moveUp();
	}
	if(key[1]){
		player1.moveDown();
	}
	if(key[2]){
		player2.moveUp();
	}
	if(key[3]){
		player2.moveDown();
	}

	//Movimiento de la bola

	ball1.move(player1.getRect(),player2.getRect(),effect1);

	if(ball1.isOut()==2){
		player2.incPoint();
		ball1.reSet(400,300,10,10,3,3,effect2);
	}
	if(ball1.isOut()==1){
		player1.incPoint();
		ball1.reSet(400,300,10,10,3,3,effect2);
	}

	//Render

	SDL_BlitSurface(field,NULL,screen,&screen->clip_rect);
	player1.show();
	player2.show();
	ball1.show();

	tmp.x=800-40;
	sprintf(c,"%d",player1.getPoints());
	text=TTF_RenderText_Solid(font,c,color);
	SDL_BlitSurface(text,NULL,screen,&tmp);


	tmp.x=0;
	sprintf(c,"%d",player2.getPoints());
	text=TTF_RenderText_Solid(font,c,color);
	SDL_BlitSurface(text,NULL,screen,&tmp);

	SDL_Flip(screen);

	if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
	}//Aqui termina el while(run)
	Mix_FreeChunk(effect1);
	Mix_FreeChunk(effect2);
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_FreeSurface(icon);
	SDL_FreeSurface(field);
	SDL_FreeSurface(text);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return 0;

}
