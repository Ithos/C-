#include "smallBullet.h"


smallBullet::smallBullet(SDL_Surface *img,int x,int y,int xvel,int yvel,int d){
	box.x=x;
	box.y=y;
	box.w=9;
	box.h=5;
	image=img;
	Xvel=xvel;
	Yvel=yvel;
	dir=d;
	if(d=='l')frame=0;
	if(d=='r')frame=3.5;
	for(int i=0;i<8;i++){
		clips[i].x=i*9;
		clips[i].y=0;
		clips[i].h=5;
		clips[i].w=9;
	}
}


void smallBullet::show(SDL_Surface *screen){
	SDL_Rect tmp={box.x-coord.x,box.y-coord.y,9,5};
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&tmp);
}

void smallBullet::move(){
	box.x+=Xvel;
	box.y+=Yvel;
	frame+=0.2;
	if(dir=='l')if(frame>3.4)frame=0.0;
	if(dir=='r')if(frame>7.4)frame=3.5;
	
}

SDL_Rect* smallBullet::getRect(){
	return &box;
}

bool smallBullet::bulletCollision(const std::vector<std::vector<int> >& map){
	//Checkeamos choques contra el mapa
	int srt=(baseclass::coord.x-(baseclass::coord.x%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Dividimos la pantalla en cuadros y recorremos solo los cuadros de la pantalla para optimizar
	int end=(baseclass::coord.x+baseclass::coord.w+(baseclass::TILE_SIZE-(baseclass::coord.x+baseclass::coord.w)%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Todo este codigo es para asegurar que los numeros que obtenemos son enteros
	
	if(srt<0)srt=0;//Esto es para que no leamos fuera de los vectores
	if(end>map[0].size())end=map[0].size();
	
	for(int i=0;i<map.size();i++){//Conviene revisar las colisiones
		for(int j=srt;j<end;j++){
				
			if(map[i][j]!=0){
					
					SDL_Rect destrect={j*50,i*50,50,50};
						if(collision(&destrect,&box)){//Si alguna bala se choca da true
							
							return true;
						
						}
					
				}
				
			}
			
		}
		//si ninguna se choca da false
		return false;
}

int smallBullet::getXvel(){
	return Xvel;
}

int smallBullet::getYvel(){
	return Yvel;
}

void smallBullet::setXvel(int vel){
	Xvel=vel;
}

void smallBullet::setYvel(int vel){
	Yvel=vel;
}

bool smallBullet::obstacleCollision(const std::vector<obstacle*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x,obstacles[i]->getRect()->y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				return true;
			}
		}
	}
	return false;
}

bool smallBullet::movCubeCollision(const std::vector<movingCube*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x,obstacles[i]->getRect()->y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				return true;
			}
		}
	}
	return false;
}

bool smallBullet::fallCubeCollision(const std::vector<fallingCube*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x,obstacles[i]->getRect()->y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				return true;
			}
		}
	}
	return false;
}
