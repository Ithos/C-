#include "enemyBullet.h"

enemyBullet::enemyBullet(SDL_Surface *img,int x,int y,int xvel,int yvel){
	box.x=x;
	box.y=y;
	box.w=box.h=20;
	image=img;
	Xvel=xvel;
	Yvel=yvel;
	frame=0;
	for(int i=0;i<3;i++){
		clips[i].x=i*20;
		clips[i].y=0;
		clips[i].h=20;
		clips[i].w=20;
	}
}


void enemyBullet::show(SDL_Surface *screen){
	SDL_Rect tmp={box.x-coord.x,box.y-coord.y,20,20};
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&tmp);
}

void enemyBullet::move(int playerX, int playerY){
	box.x+=Xvel;
	box.y+=Yvel;
	
	if(playerX<box.x-coord.x && Xvel>=-10)Xvel--;//Actualizamos velocidad
	if(playerX>box.x-coord.x && Xvel<=10)Xvel++;
	
	if(playerY<box.y-coord.y && Yvel>=-5)Yvel--;
	if(playerY>box.y-coord.y && Yvel<=5)Yvel++;
	
	frame+=0.2;
	if(frame>2.4)frame=0.0;
}

SDL_Rect* enemyBullet::getRect(){
	return &box;
}

bool enemyBullet::bulletCollision(const std::vector<std::vector<int> >& map){
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

int enemyBullet::getXvel(){
	return Xvel;
}

int enemyBullet::getYvel(){
	return Yvel;
}

void enemyBullet::setXvel(int vel){
	Xvel=vel;
}

void enemyBullet::setYvel(int vel){
	Yvel=vel;
}

bool enemyBullet::obstacleCollision(const std::vector<obstacle*>& obstacles){
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

bool enemyBullet::movCubeCollision(const std::vector<movingCube*>& obstacles){
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

bool enemyBullet::fallCubeCollision(const std::vector<fallingCube*>& obstacles){
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
