#include "menu.h"

menu::menu(){	
}

menu::~menu(){
	for(int i=0;i<NUMENU;i++)SDL_FreeSurface(menus[i]);	
}

int menu::show(SDL_Surface* screen,TTF_Font* font){
	Uint32 time;
	const int FPS=30;
	int x,y;
	bool run=true;
	bool selected[NUMENU]={0,0};
	SDL_Event event;
	const char* labels[NUMENU]={"Continue","Exit"};
	SDL_Color color[2]={{255,255,255},{100,0,255}};
	//Establecemos la transparencia del fondo del menú
	SDL_Surface* alpha=SDL_CreateRGBSurface(SDL_SWSURFACE,screen->w,screen->h,32,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);
	SDL_FillRect(alpha,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0));
	SDL_SetAlpha(alpha,SDL_SRCALPHA,129);//Indicamos el nivel de transparencia siendo 150 opaco
	SDL_BlitSurface(alpha,NULL,screen,NULL);
	SDL_FreeSurface(alpha);
	

	menus[0]=TTF_RenderText_Solid(font,labels[0],color[0]);
	menus[1]=TTF_RenderText_Solid(font,labels[1],color[0]);

	pos[0].x=screen->clip_rect.w/2-menus[0]->clip_rect.w/2;
	pos[0].y=screen->clip_rect.h/2-menus[0]->clip_rect.h;
	
	pos[1].x=screen->clip_rect.w/2-menus[1]->clip_rect.w/2;
	pos[1].y=screen->clip_rect.h/2+menus[1]->clip_rect.h;

		

	while(run){
		time=SDL_GetTicks();
	while(SDL_PollEvent(&event)){
		if(event.type==SDL_QUIT)return 1;
		if(event.type==SDL_MOUSEMOTION){
			x=event.motion.x;
			y=event.motion.y;
			for(int i=0;i<NUMENU;i++){
				if(x>pos[i].x&&x<(pos[i].x+pos[i].w)&&y>pos[i].y&&y<(pos[i].y+pos[i].h)){
					if(!selected[i]){
						selected[i]=true;
						SDL_FreeSurface(menus[i]);
						menus[i]=TTF_RenderText_Solid(font,labels[i],color[1]);
					}
				}else{
					if(selected[i]){
						selected[i]=false;
						SDL_FreeSurface(menus[i]);
						menus[i]=TTF_RenderText_Solid(font,labels[i],color[0]);
					}
				}
			}


		}//Fin del mousemotion
		if(event.type==SDL_MOUSEBUTTONDOWN){
			x=event.button.x;
			y=event.button.y;
			for(int i=0;i<NUMENU;i++){
				if(x>pos[i].x&&x<(pos[i].x+pos[i].w)&&y>pos[i].y&&y<(pos[i].y+pos[i].h)&&event.button.button==SDL_BUTTON_LEFT){
					return i;
				}
			}
		}
		if(event.type==SDL_KEYDOWN){
			if(event.key.keysym.sym==SDLK_ESCAPE)return 0;
		}
		

	}//Aqui acaba el while event
	for(int i=0;i<NUMENU;i++)SDL_BlitSurface(menus[i],NULL,screen,&pos[i]);
	SDL_Flip(screen);
	if ((1000/FPS)>(SDL_GetTicks()-time)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-time));//de esta manera reducimos el uso del procesador
	} 

	}//Aqui acaba el while
}
