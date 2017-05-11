#include<SDL/SDL.h>

int main (int argc, char** argv){
	
	int a=3,b=2;
	bool run=true;
	bool key[4]={0,0,0,0};
	Uint32 start; //Integer 32 bits	
	const int FPS=30;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen;//SDL_Surface es un struct
	screen=SDL_SetVideoMode(800,600,32,SDL_SWSURFACE);
	SDL_WM_SetCaption( "Pong", "Pong" );
	
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
	lim_sup.x=90;
	lim_sup.y=60;
	lim_sup.w=630;
	lim_sup.h=10;
	Uint32 negro=SDL_MapRGB(screen->format,0x00,0x00,0x00); //Definimos un color
	SDL_Rect lim_inf;
	lim_inf.x=90;
	lim_inf.y=540;
	lim_inf.w=630;
	lim_inf.h=10;

	
	while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_UP)key[0]=1;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=1;
				if(event.key.keysym.sym==SDLK_w)key[2]=1;
				if(event.key.keysym.sym==SDLK_z)key[3]=1;
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP)key[0]=0;
				if(event.key.keysym.sym==SDLK_DOWN)key[1]=0;
				if(event.key.keysym.sym==SDLK_w)key[2]=0;
				if(event.key.keysym.sym==SDLK_z)key[3]=0;
			}
		}//A partir de aquí empieza la logica del programa
		if(key[0]&&pala2.y!=70)pala2.y=pala2.y-2;
		if(key[1]&&pala2.y!=490)pala2.y=pala2.y+2;
		if(key[2]&&pala1.y!=70)pala1.y=pala1.y-2;
		if(key[3]&&pala1.y!=490)pala1.y=pala1.y+2;
		
		//Física de la bola
		if((bola.x<=(pala1.x+10))&&(bola.y>=pala1.y)&&(bola.y<=pala1.y+50)&&((bola.x+10)>=pala1.x)){
			a=-a;
			if(key[2])b=b-2;
			if(key[3])b=b+2;
		}
		if(((bola.x+10)>=pala2.x)&&(bola.y>=pala2.y)&&(bola.y<=pala2.y+50)&&(bola.x<=(pala2.x+10))){
			a=-a;
			if(key[0])b=b-2;
			if(key[1])b=b+2;
		}
		if((bola.x<=(pala1.x+6))&&((bola.x+10)>=pala1.x)&&(bola.y<=(pala1.y+50))&&((bola.y+10)>=pala1.y)){
			a=-a;
			if(bola.y<=(pala1.y+50))b=-b+2;
			if((bola.y+10)>=pala1.y)b=-b-2;
		}
		if(((bola.x+10)>=(pala2.x+4))&&((bola.y+10)>=pala2.y)&&(bola.y<=pala2.y+50)&&(bola.x<=(pala2.x+10))){
			a=-a;
			if((bola.y+10)>=pala2.y)b=-b-2;
			if(bola.y<=(pala2.y+50))b=-b+2;
		}
		if(bola.y>=lim_inf.y-10)b=-b;
		if(bola.y<=lim_sup.y+10)b=-b;
		if(bola.x<=0){
			bola.x=400;
			bola.y=300;
			bola.w=10;
			bola.h=10;
			a=3;
			b=2;
		}
		if(bola.x>=800){
			bola.x=400;
			bola.y=300;
			bola.w=10;
			bola.h=10;
			a=-3;
			b=2;
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



		SDL_Flip(screen);
		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
		
	}
	SDL_Quit();
	return 0;
}
