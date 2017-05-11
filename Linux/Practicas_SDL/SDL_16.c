#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

//**************************************************************************************************************************************
class particle{//En aplicaciones más profesionales esta clase iria en otro documento y la importariamos
	float x,y,xvel,yvel;
	Uint32 endTime;
	Uint8 color;
	public:
	particle(float X,float Y,float XVel,float YVel,int life,Uint8 color);
	void move();
	void show();
	bool isDead();
	
};

particle::particle(float X,float Y,float XVel,float YVel,int life,Uint8 COLOR){
	x=X;
	y=Y;
	xvel=XVel;
	yvel=YVel;
	endTime=SDL_GetTicks()+life;
	color=COLOR;
};

void particle::move(){
	x+=xvel+rand()%3-1;
	y+=yvel+rand()%3-1;
	
	if(x<0)x=0;
	if(y<0)y=0;
	if(x>=SDL_GetVideoSurface()->w)x=SDL_GetVideoSurface()->w-1;
	if(y>=SDL_GetVideoSurface()->h)y=SDL_GetVideoSurface()->h-1;
	
};

void particle::show(){//Si pasamos a 32bits los colores esto hay que modificarlo
	Uint8* pixels=(Uint8*)SDL_GetVideoSurface()->pixels;
	Uint8* pixel=pixels+(int)y*SDL_GetVideoSurface()->pitch+(int)x;
	*pixel=color;
};

bool particle::isDead(){
	return ((SDL_GetTicks()>=endTime)||x==0||y==0||(x==SDL_GetVideoSurface()->w-1)||(y==SDL_GetVideoSurface()->h-1));
}
//****************************************************************************************************************************************
class particleEngine{//Otra clase que deberiamos crear en otro documento
	std::vector<particle*> particles;
	int x,y,maxparticles;
	public:
	particleEngine(int X,int Y,int MAXparticles);
	~particleEngine();
	void refresh();
	
};

particleEngine::particleEngine(int X,int Y,int MAXparticles){
	x=X;
	y=Y;
	maxparticles=MAXparticles;
	for(int i=0;i<maxparticles;i++)particles.push_back(new particle(x+rand()%5-2,y+rand()%5-2,rand()%3-1,-(2+rand()%6),rand()%100,SDL_MapRGB(SDL_GetVideoSurface()->format,0xff,0x00,0x00)));
	//En el for llenamos el vector de partículas
};

particleEngine::~particleEngine(){
	for(int i=0;i<maxparticles;i++)delete particles[i];
};

void particleEngine::refresh(){
	for(int i=0;i<maxparticles;i++){
		if(particles[i]->isDead()){//si la particula ha muerto la borramos y creamos otra nueva
			delete particles[i];
			particles[i]=new particle(x+rand()%5-2,y+rand()%5-2,rand()%3-1,-(2+rand()%6),rand()%400,SDL_MapRGB(SDL_GetVideoSurface()->format,0xff,0x00,0x00));
		}else{
			particles[i]->move();
			particles[i]->show();
		}
	}
	
};
//*************************************************************************************************************************************


int main (int argc, char** argv){

SDL_Init(SDL_INIT_EVERYTHING);
SDL_Surface* screen;
screen=SDL_SetVideoMode(640,480,8,SDL_SWSURFACE);//Empezamos probando con 8 colores
SDL_WM_SetCaption( "Hello_SDL", "Hello_SDL" );
bool run=true;
const int FPS=30;
Uint32 start; //Integer 32 bits	

srand(time(0));
particleEngine ps(screen->w/2,screen->h/2,1000);

while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
			}
			if(event.type==SDL_KEYUP){
			}
		}
		
		
		SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));
		ps.refresh();//Para crear fuego en condicione habria que superponer varios motores de particulas
		SDL_Flip(screen);
		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
}
SDL_Quit();
return 0;
}
