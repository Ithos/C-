#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_mixer.h>

int main (int argc, char** argv){
	
	bool run=true;
	bool key[4]={0,0,0,0};
	Uint32 start; //Integer 32 bits	
	const int FPS=30;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen;//SDL_Surface es un struct
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption( "Hello_SDL", "Hello_SDL" );
	
	SDL_Surface* tmp=IMG_Load("spaceship.bmp");//Cargamos la imagen
	SDL_Surface* image=SDL_DisplayFormat(tmp);//Creo que esto convierte a la imagen en una especie de SDL_Rect
	SDL_FreeSurface(tmp);//Eliminamos tmp
	SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,77,77,77));
	
	float x=300,y=200;
	
	int angle=0;//Angulo que rotamos
	int lastangle=0;
	
	SDL_Surface *rotatedimage=rotozoomSurface(image,angle,1.0,0);//Precreamos la imagenrotada para optimizar
	
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
		if(key[0]){
			//up
			x+=sin(angle*M_PI/180)*3;
			y+=cos(angle*M_PI/180)*3;
		}
		if(key[1]){
			//down
			x-=sin(angle*M_PI/180)*3;
			y-=cos(angle*M_PI/180)*3;
		}
		if(key[2]){
			//right
			angle+=3;
		}
		if(key[3]){
			//left
			angle-=3;
		}
		
		if(angle>=360)angle-=360;//Maximos y minimos para el angulo
		if(angle<=-360)angle+=360;
		
		if(lastangle!=angle){
			SDL_FreeSurface(rotatedimage);//Eliminamos la imagen rotada anterior
			rotatedimage=rotozoomSurface(image,angle,1.0,0);//creamos la nueva
			}
		SDL_Rect rec={(int)x,(int)y,100,150};//El rectangulo en el que dibujamos
		rec.x-=rotatedimage->w/2-image->w/2;
		rec.y-=rotatedimage->h/2-image->w/2;//Esto es para centrar la rotación, al rotar desplazamos el centro de la imagen, de esta forma lo evitamos restando la diferencia entre el centro nuevo y el anterior
		
		
		SDL_FillRect(screen,NULL,0);
		SDL_BlitSurface(rotatedimage,NULL,screen,&rec);
		
		lastangle=angle;
		SDL_Flip(screen);
		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
		
	}
	SDL_FreeSurface(image);
	SDL_Quit();
	return 0;
}
