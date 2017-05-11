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
	int getX();
	int getY();
	void setX(int X);
	void setY(int Y);
	
};

particleEngine::particleEngine(int X,int Y,int MAXparticles){
	x=X;
	y=Y;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x>=SDL_GetVideoSurface()->w)x=SDL_GetVideoSurface()->w-1;
	if(y>=SDL_GetVideoSurface()->h)y=SDL_GetVideoSurface()->h-1;
	maxparticles=MAXparticles;
	for(int i=0;i<maxparticles;i++)particles.push_back(new particle(x+rand()%11-5,y+rand()%11-5,rand()%5-2+((float)rand()/(float)RAND_MAX),-(rand()%11+2+((float)rand()/(float)RAND_MAX)),100+rand()%100,/*rand()%255*/SDL_MapRGB(SDL_GetVideoSurface()->format,0xff,0x00,0x00)));
	//En el for llenamos el vector de partículas
};

particleEngine::~particleEngine(){
	for(int i=0;i<maxparticles;i++)delete particles[i];
};

void particleEngine::refresh(){
	for(int i=0;i<maxparticles;i++){
		if(particles[i]->isDead()){//si la particula ha muerto la borramos y creamos otra nueva
			delete particles[i];
			particles[i]=new particle(x+rand()%11-5,y+rand()%11-5,rand()%5-2+((float)rand()/(float)RAND_MAX),-(rand()%11+2+((float)rand()/(float)RAND_MAX)),rand()%51*(rand()%4+1),/*rand()%255*/SDL_MapRGB(SDL_GetVideoSurface()->format,0xff,0x00,0x00));
		}else{//Al incluir los floats aumentamos la aleatoriedad y al multiplicar dos funciones aleatorias variamos la distribucion de probabilidad, y por lo tanto la distribucion de las particulas
			particles[i]->move();
			particles[i]->show();
		}
	}
	
};

int particleEngine::getX(){
	return x;
};
int particleEngine::getY(){
	return y;
};
void particleEngine::setX(int X){
	x=X;
	if(x<0)x=0;
	if(x>=SDL_GetVideoSurface()->w)x=SDL_GetVideoSurface()->w-1;
};
void particleEngine::setY(int Y){
	y=Y;
	if(y<0)y=0;
	if(y>=SDL_GetVideoSurface()->h)y=SDL_GetVideoSurface()->h-1;
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
particleEngine ps(screen->w/2,screen->h/2,500);
bool key[4]={0,0,0,0};

while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		SDL_Event event; //si ocurre un evento lo llevamos a event
		while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_UP){key[0]=1;}
				if(event.key.keysym.sym==SDLK_DOWN){key[1]=1;}
				if(event.key.keysym.sym==SDLK_RIGHT){key[2]=1;}
				if(event.key.keysym.sym==SDLK_LEFT){key[3]=1;}
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP){key[0]=0;}
				if(event.key.keysym.sym==SDLK_DOWN){key[1]=0;}
				if(event.key.keysym.sym==SDLK_RIGHT){key[2]=0;}
				if(event.key.keysym.sym==SDLK_LEFT){key[3]=0;}
			}
		}
		//Podríamos mejorar el programa haciendo que las particulas se viesen afectadas por la gravedad o viento 
		
		if(key[0]){
			ps.setY(ps.getY()-3);
			if(ps.getY()<0)ps.setY(0);
		}
		if(key[1]){
			ps.setY(ps.getY()+3);
			if(ps.getY()>=SDL_GetVideoSurface()->h)ps.setY(SDL_GetVideoSurface()->h-1);
		}
		if(key[2]){
			ps.setX(ps.getX()+3);
			if(ps.getX()>=SDL_GetVideoSurface()->w)ps.setX(SDL_GetVideoSurface()->w-1);
		}
		if(key[3]){
			ps.setX(ps.getX()-3);
			if(ps.getX()<0)ps.setX(0);
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
