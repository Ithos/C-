#include<SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <SDL/SDL_mixer.h>

bool Collision(SDL_Rect* rect1, SDL_Rect* rect2){//Chequeamos cuando no hay colisión y devolvemos 0, si no devolvemos 1
	if(rect1->y >= rect2->y + rect2->h)return 0;
	if(rect1->x >= rect2->x + rect2->w)return 0;
	if(rect1->y + rect1->h <= rect2->y)return 0;
	if(rect1->x + rect1->w <= rect2->x)return 0;
	return 1;		
}

void checkScreen (SDL_Rect* rect, SDL_Surface* screen){
	if(rect->x < 0)rect->x=0;
	if(rect->y < 0)rect->y=0;
	if(rect->x + rect->w > screen->w)rect->x=screen->w - rect->w;
	if(rect->y + rect->h > screen->h)rect->y=screen->h - rect->h;
}


void setRects (SDL_Rect* clip){
	clip[0].x=0;
	clip[0].y=0;
	clip[0].w=87;
	clip[0].h=112;

	clip[1].x=87;
	clip[1].y=0;
	clip[1].w=87;
	clip[1].h=112;

	clip[2].x=174;
	clip[2].y=0;
	clip[2].w=87;
	clip[2].h=112;

	clip[3].x=0;
	clip[3].y=112;
	clip[3].w=87;
	clip[3].h=112;

	clip[4].x=87;
	clip[4].y=112;
	clip[4].w=87;
	clip[4].h=112;

	clip[5].x=174;
	clip[5].y=112;
	clip[5].w=87;
	clip[5].h=112;
}
void setRectarray (SDL_Rect* clip){
	int i=0;
	for(i=0;i<4;i++){
	clip[i].x=i*100;
	clip[i].y=i*70;
	clip[i].w=20;
	clip[i].h=20;
	}
	

}


int main (int argc, char** argv){
	
	bool run=true;
	bool key[4]={0,0,0,0};
	bool key_anim[4]={0,0,0,0};
	Uint32 start; //Integer 32 bits	
	const int FPS=30;
	int i;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen, *image;//SDL_Surface es un struct
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption( "Hello_SDL", "Hello_SDL" );

	Mix_Chunk* effect1;
	Mix_Chunk* effect2;
	Mix_Music* music;
	Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
	music=Mix_LoadMUS("A-One-Winged-Angel.mp3");
	effect1=Mix_LoadWAV("Prelude.wav");
	effect2=Mix_LoadWAV("Fanfare.wav");
	
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
	Uint32 color2=SDL_MapRGB(screen->format,0xff,0x22,0xdd); //Definimos un color
	Uint32 color3=SDL_MapRGB(screen->format,0x23,0x55,0xcc); //Definimos un color
	Uint32 color4=SDL_MapRGB(screen->format,0xff,0xff,0xff); //Definimos un color



	image=IMG_Load("animation.bmp");
	SDL_Rect rects[6];
	setRects(rects);
	int frame=0;
	int doub=0;



	SDL_Rect rect;
	rect.x=200;
	rect.y=150;
	rect.h=600;
	rect.w=300;
	SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,254,254,254));

	SDL_Rect rectarray[4];
	setRectarray(rectarray);

	
	Mix_PlayMusic(music,-1);//El segundo parametro es el numero de veces que se repite, -1 es siempre, 0 es 1 vez, 1 es 2 veces, etc..
	 Mix_VolumeMusic(70);//de 0 a 128	

	while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_UP){key[0]=1;Mix_PlayChannel(0,effect1,-1);}//en -1 el canal se selecciona
				if(event.key.keysym.sym==SDLK_DOWN){key[1]=1;Mix_PlayChannel(1,effect1,-1);}//automaticamente
				if(event.key.keysym.sym==SDLK_RIGHT){key[2]=1;Mix_PlayChannel(2,effect1,-1);}//si se selecciona un canal
				if(event.key.keysym.sym==SDLK_LEFT){key[3]=1;Mix_PlayChannel(3,effect1,-1);}//cuando se intenta volver a usar
				if(event.key.keysym.sym==SDLK_w){key_anim[0]=1;Mix_PlayChannel(4,effect2,-1);}//la musica que estaba sonando
				if(event.key.keysym.sym==SDLK_s){key_anim[1]=1;Mix_PlayChannel(5,effect2,-1);}//para 
				if(event.key.keysym.sym==SDLK_d){key_anim[2]=1;Mix_PlayChannel(6,effect2,-1);}
				if(event.key.keysym.sym==SDLK_a){key_anim[3]=1;Mix_PlayChannel(7,effect2,-1);}
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP){key[0]=0;Mix_FadeOutChannel(0,20);}
				if(event.key.keysym.sym==SDLK_DOWN){key[1]=0;Mix_FadeOutChannel(1,20);}
				if(event.key.keysym.sym==SDLK_RIGHT){key[2]=0;Mix_FadeOutChannel(2,20);}
				if(event.key.keysym.sym==SDLK_LEFT){key[3]=0;Mix_FadeOutChannel(3,20);}
				if(event.key.keysym.sym==SDLK_w){key_anim[0]=0;Mix_FadeOutChannel(4,20);}
				if(event.key.keysym.sym==SDLK_s){key_anim[1]=0;Mix_FadeOutChannel(5,20);}
				if(event.key.keysym.sym==SDLK_d){key_anim[2]=0;Mix_FadeOutChannel(6,20);}
				if(event.key.keysym.sym==SDLK_a){key_anim[3]=0;Mix_FadeOutChannel(7,20);}
			}
		}


		//A partir de aquí empieza la logica del programa
		if(key[0]){
			rect1.y--;
			if(Collision(&rect1,&rect2)||Collision(&rect1,&rect))rect1.y++;
			for(i=0;i<4;i++){
				if(Collision(&rect1,&rectarray[i]))rect1.y++;
			}
		}
		if(key[1]){
			rect1.y++;
			if(Collision(&rect1,&rect2)||Collision(&rect1,&rect))rect1.y--;
			for(i=0;i<4;i++){
				if(Collision(&rect1,&rectarray[i]))rect1.y--;
			}
		}
		if(key[2]){
			rect1.x++;
			if(Collision(&rect1,&rect2)||Collision(&rect1,&rect))rect1.x--;
			for(i=0;i<4;i++){
				if(Collision(&rect1,&rectarray[i]))rect1.x--;
			}
		}
		if(key[3]){
			rect1.x--;
			if(Collision(&rect1,&rect2)||Collision(&rect1,&rect))rect1.x++;
			for(i=0;i<4;i++){
				if(Collision(&rect1,&rectarray[i]))rect1.x++;
			}
		}


		if(key_anim[0]){
			rect.y--;
			if(Collision(&rect,&rect2)||Collision(&rect,&rect1))rect.y++;
			for(i=0;i<4;i++){
				if(Collision(&rect,&rectarray[i]))rect.y++;
			}
		}
		if(key_anim[1]){
			Mix_PlayChannel(-2,effect2,0);
			rect.y++;
			if(Collision(&rect,&rect2)||Collision(&rect,&rect1))rect.y--;
			for(i=0;i<4;i++){
				if(Collision(&rect,&rectarray[i]))rect.y--;
			}
		}
		if(key_anim[2]){
			rect.x++;
			if(Collision(&rect,&rect2)||Collision(&rect,&rect1))rect.x--;
			for(i=0;i<4;i++){
				if(Collision(&rect,&rectarray[i]))rect.x--;
			}
		}
		if(key_anim[3]){
			rect.x--;
			if(Collision(&rect,&rect2)||Collision(&rect,&rect1))rect.x++;
			for(i=0;i<4;i++){
				if(Collision(&rect,&rectarray[i]))rect.x++;
			}
		}


		checkScreen(&rect1,screen);
		checkScreen(&rect,screen);



		
		if(key_anim[0]||key_anim[1]||key_anim[2]||key_anim[3]){
			doub++;
			if(doub==2){frame++;doub=0;}//se puede meter en la zona de logica y hacer que dependa de la tecla que se pulse
			if(frame>5)frame=0;
		}else{
			frame=1;
		}


		//Aqui dibujamos
		
		SDL_FillRect(screen,&screen->clip_rect,color3);//Dibuja por orden, asique en este caso sobre el fondo verde se dibuja todo
		SDL_BlitSurface(image,&rects[frame],screen,&rect);
		SDL_FillRect(screen,&rect1,color1);//Si lo haces mal dibujas el fondo sobre otras cosas y no se ven
		SDL_FillRect(screen,&rect2,color2);
		for(i=0;i<4;i++){
			SDL_FillRect(screen,&rectarray[i],color4);
		}
		
		
		SDL_Flip(screen);
		


		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
		
	}
	SDL_FreeSurface(image);
	Mix_FreeChunk(effect1);
	Mix_FreeChunk(effect2);
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}
