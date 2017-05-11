#include <SDL/SDL.h>
#include <iostream>

void pixelShaper(SDL_Surface* screen, int x, int y,int r,int g,int b){
	if(r>255)r=255;
	if(g>255)g=255;
	if(b>255)b=255;
	if(r<0)r=0;
	if(g<0)g=0;
	if(b<0)b=0;
	if(x>=0&&x<=screen->w&&y>=0&&y<=screen->h){
		((Uint32*)screen->pixels)[y*screen->pitch/4+x]=SDL_MapRGB(screen->format,r,g,b);
	}
}

void swap(int &a,int &b){//Empleamos referencias para que cambien los valores de las variables reales
	int tmp=a;
	a=b;
	b=tmp;
}

void drawLine(SDL_Surface* screen,int x0,int y0,int x1,int y1){
	bool step=abs(x1-x0)<abs(y1-y0);//Si la línea tiene un angulo mayor en valor absoluto que 45º la rotamos (esto es por el algoritmo de Besenham)
	
	if(step){//si step rotamos
		swap(x0,y0);
		swap(x1,y1);
	}
	
	if(x0>x1){//si la línea va de derecha a izquierda cambiamos los valores iniciales por los finales para que vaya de izq a der
		swap(x0,x1);
		swap(y0,y1);
	}
	
	/*Algoritmo de Besenham*/ //Este algoritmo dibuja una línea pixel a pixel dados un punto inicial y final
	float err=0.0;
	float roundErr=(float)abs(y1-y0)/(x1-x0);
	int y=y0;
	int ystep=(y1>y0?1:-1);//Segun si la linea va para arriba o para abajo
	
	for(int i=x0;i<x1;i++){
		
		if(step)pixelShaper(screen,y,i,255,255,255);//si tuvimos que rotar la linea deshacemos la rotación
		else pixelShaper(screen,i,y,255,255,255);
		
		err+=roundErr;
		if(err>=0.5){
			y+=ystep;
			err-=1.0;
		}
	}
	
	
}

int main (int argc, char** argv){

SDL_Init(SDL_INIT_EVERYTHING);
SDL_Surface* screen;
screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);//Empezamos probando con 8 colores
SDL_WM_SetCaption( "Hello_SDL", "Hello_SDL" );
bool run=true;
const int FPS=30;
Uint32 start; //Integer 32 bits

int mx=10,my=10;

while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_MOUSEMOTION){
				mx=event.motion.x;
				my=event.motion.y;
			}
		}
		
		
		SDL_FillRect(screen,NULL,0);
		drawLine(screen,mx,my,screen->w/2,screen->h/2);
		SDL_Flip(screen);
		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
}
SDL_Quit();
return 0;
}
