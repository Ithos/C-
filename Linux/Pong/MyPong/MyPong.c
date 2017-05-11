//Álvaro Pérez Corral		11/01/2013		Thanks to Cplusplusguy for his tutorials//

#include<SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>


bool Collision(SDL_Rect* rect1, SDL_Rect* rect2){//Chequeamos cuando no hay colisión y devolvemos 0, si no devolvemos 1
	if(rect1->y >= rect2->y + rect2->h)return 0;
	if(rect1->x >= rect2->x + rect2->w)return 0;
	if(rect1->y + rect1->h <= rect2->y)return 0;
	if(rect1->x + rect1->w <= rect2->x)return 0;
	return 1;		
}


int main (int argc, char** argv){
	
	int a=3,b=1;
	char m01=48,m11=48,m02=48,m12=48;
	bool run=true;
	bool key[4]={0,0,0,0};
	Uint32 start; //Integer 32 bits	
	const int FPS=30;
	char wtext[]={m02,m12,' ','|',' ',m01,m11};

	SDL_Init(SDL_INIT_EVERYTHING);
	
	//Iniciamos la librería TTF y cargamos la fuente a emplear
	TTF_Init();
	TTF_Font* font=	TTF_OpenFont("DroidSansMono.ttf",32);
	SDL_Surface* text;//Donde vamos a escribir


	//Creamos la ventana y le damos un nombre
	SDL_Surface* screen;//SDL_Surface es un struct
	screen=SDL_SetVideoMode(800,600,32,SDL_SWSURFACE);
	SDL_WM_SetCaption( "Pong", "Pong" );

	//Ponemos la música y los efectos
	Mix_Chunk* effect1;
	Mix_Chunk* effect2;
	Mix_Music* music;
	Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
	music=Mix_LoadMUS("Fisherman's Horizon.mp3");
	effect1=Mix_LoadWAV("efect.wav");
	effect2=Mix_LoadWAV("efect2.wav");	

	//Definimos las formas a dibujar y los colores

	SDL_Rect bola;
	bola.x=400;
	bola.y=300;
	bola.w=10;
	bola.h=10;
	Uint32 blanco=SDL_MapRGB(screen->format,0xff,0xff,0xff); //Definimos un color
	SDL_Rect pala1;
	pala1.x=80;
	pala1.y=300;
	pala1.w=10;
	pala1.h=50;
	Uint32 rojo=SDL_MapRGB(screen->format,0xff,0x00,0x00); //Definimos un color
	SDL_Rect pala2;
	pala2.x=720;
	pala2.y=300;
	pala2.w=10;
	pala2.h=50;
	Uint32 azul=SDL_MapRGB(screen->format,0x00,0x00,0xff); //Definimos un color
	SDL_Rect lim_sup;
	lim_sup.x=80;
	lim_sup.y=60;
	lim_sup.w=650;
	lim_sup.h=10;
	Uint32 negro=SDL_MapRGB(screen->format,0x00,0x00,0x00); //Definimos un color
	SDL_Rect lim_inf;
	lim_inf.x=80;
	lim_inf.y=540;
	lim_inf.w=650;
	lim_inf.h=10;
	SDL_Rect text_space;
	text_space.x=335;
	text_space.y=10;
	text_space.w=70;
	text_space.h=40;
	SDL_Color c_blanco={255,255,255};

	//Iniciamos la música y establecemos el volumen.
	Mix_PlayMusic(music,-1);
	 Mix_VolumeMusic(10);//de 0 a 128
	
	
	while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_UP)key[0]=1;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=1;
				if(event.key.keysym.sym==SDLK_w)key[2]=1;
				if(event.key.keysym.sym==SDLK_s)key[3]=1;
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP)key[0]=0;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=0;
				if(event.key.keysym.sym==SDLK_w)key[2]=0;
				if(event.key.keysym.sym==SDLK_s)key[3]=0;
			}
		}//A partir de aquí empieza la logica del programa


		//Definimos el texto a escribir
		wtext[0]=m02;
		wtext[1]=m12;
		wtext[5]=m01;
		wtext[6]=m11;
		text=TTF_RenderText_Solid(font,wtext,c_blanco);		


		//Movimiento de las palas

		if(key[0]){
			pala2.y=pala2.y-2;
			if(Collision(&pala2,&lim_sup))pala2.y=pala2.y+2;
		}


		if(key[1]){
			pala2.y=pala2.y+2;
			if(Collision(&pala2,&lim_inf))pala2.y=pala2.y-2;
		}


		if(key[2]){
			pala1.y=pala1.y-2;
			if(Collision(&pala1,&lim_sup))pala1.y=pala1.y+2;
		}

		if(key[3]){
			pala1.y=pala1.y+2;
			if(Collision(&pala1,&lim_inf))pala1.y=pala1.y-2;
		}
		
		//Física de la bola
		if(Collision(&bola,&pala1)){
			Mix_PlayChannel(0,effect1,1);
			a=-a;
			if(bola.y>pala1.y&&bola.x<(pala1.x+pala1.w-3)){
				bola.x=pala1.x+pala1.w;			
				b=-b;
			}else{
				if((bola.y+bola.h)<pala1.y&&bola.x<(pala1.x+pala1.w-3)){
					bola.x=pala1.x+pala1.w;
					b=-b;
				}else{
					if(key[2])b=b-2;
					if(key[3])b=b+2;
				}
			}
		}
		if(Collision(&bola,&pala2)){
			Mix_PlayChannel(0,effect1,1);
			a=-a;
			if(bola.y>pala2.y&&(bola.x+bola.w-3)>pala2.x){	
				bola.x=pala2.x-bola.w;		
				b=-b;
			}else{
				if((bola.y+bola.h)<pala2.y&&(bola.x+bola.w-3)>pala2.x){
					bola.x=pala2.x-bola.w;
					b=-b;
				}else{		
					if(key[0])b=b-2;
					if(key[1])b=b+2;
				}
			}
		}


		if(Collision(&bola,&lim_inf)){
			b=-b;
			Mix_PlayChannel(0,effect1,1);
		}

		if(Collision(&bola,&lim_sup)){
			b=-b;
			Mix_PlayChannel(0,effect1,1);
		}


		if(bola.x<=0){
			Mix_PlayChannel(0,effect2,1);
			bola.x=400;
			bola.y=300;
			bola.w=10;
			bola.h=10;
			a=3;
			b=1;
			if(m01!=57){
				if(m11==57){
					m11=48;
					m01++;
				}else{
					m11++;
				}
			}
		}


		if(bola.x>=800){
			Mix_PlayChannel(0,effect2,1);
			bola.x=400;
			bola.y=300;
			bola.w=10;
			bola.h=10;
			a=-3;
			b=1;
			if(m02!=57){
				if(m12==57){
					m12=48;
					m02++;
				}else{
					m12++;
				}
			}
		}
		
		bola.x=bola.x+a;
		bola.y=bola.y+b;
		
			


		//Aqui dibujamos
		SDL_FillRect(screen,&screen->clip_rect,negro);//Dibuja por orden, asique en este caso sobre el fondo se dibuja todo
		SDL_FillRect(screen,&lim_sup,blanco);//Si lo haces mal dibujas el fondo sobre otras cosas y no se ven
		SDL_FillRect(screen,&lim_inf,blanco);//Estos son los limites para el campo
		SDL_FillRect(screen,&bola,blanco);
		SDL_FillRect(screen,&pala1,rojo);
		SDL_FillRect(screen,&pala2,azul);

		SDL_BlitSurface(text,NULL,screen,&text_space);//Escribimos el texto

		SDL_Flip(screen);
		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
		
	}

	//Cerramos todo y salimos
	SDL_FreeSurface(text);
	TTF_CloseFont(font);
	TTF_Quit();
	Mix_FreeChunk(effect1);
	Mix_FreeChunk(effect2);
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}
