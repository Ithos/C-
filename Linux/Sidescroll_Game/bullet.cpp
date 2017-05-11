# include "bullet.h"

bullet::bullet(SDL_Surface *img,int x,int y,int xvel,int yvel){
	box.x=x;
	box.y=y;
	box.w=box.h=8;
	image=img;
	Xvel=xvel;
	Yvel=yvel;
	frame=0;
	for(int i=0;i<3;i++){
		clips[i].x=i*8;
		clips[i].y=0;
		clips[i].h=8;
		clips[i].w=8;
	}
}

void bullet::move(int playerXvel,int playerYvel){//Simplemente actualizamos posicion
	
	if((Xvel>=0&&playerXvel>=0)||(Xvel<0&&playerXvel<0))box.x+=Xvel;
	else box.x+=Xvel-playerXvel;
	box.y+=Yvel;
	frame+=0.2;
	if(frame>2.4)frame=0.0;
	
};

void bullet::show(SDL_Surface *screen){//Mandamos la imagen a screen , habra que actualizar para la animacion
	
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&box);
	
};

SDL_Rect* bullet::getRect(){
	return &box;
};

int bullet::getXvel(){
	return Xvel;
};

int bullet::getYvel(){
	return Yvel;
};

void bullet::setXvel(int vel){
		Xvel=vel;
};

void bullet::setYvel(int vel){
	Yvel=vel;
};

bool bullet::bulletCollision(const std::vector<std::vector<int> >& map){//recibimos el mapa y cada bala chequea si se ha chocado
	//Aquí chequeamos las colisiones de las balas, hay otra versión con esto en game
		
		//REVISAR Y PASAR A COLISIONES BINARIAS//
		
	int srt=(baseclass::coord.x-(baseclass::coord.x%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Dividimos la pantalla en cuadros y recorremos solo los cuadros de la pantalla para optimizar
	int end=(baseclass::coord.x+baseclass::coord.w+(baseclass::TILE_SIZE-(baseclass::coord.x+baseclass::coord.w)%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Todo este codigo es para asegurar que los numeros que obtenemos son enteros
	
	if(srt<0)srt=0;//Esto es para que no leamos fuera de los vectores
	if(end>map[0].size())end=map[0].size();
	
	for(int i=0;i<map.size();i++){//Conviene revisar las colisiones
		for(int j=srt;j<end;j++){
				
			if(map[i][j]!=0){
					
					SDL_Rect destrect={j*50-baseclass::coord.x,i*50-baseclass::coord.y,50,50};
						if(collision(&destrect,&box)){//Si alguna bala se choca da true
							
							return true;
						
						}
					
				}
				
			}
			
		}
		//si ninguna se choca da false
		return false;
	
};

bool bullet::obstacleCollision(const std::vector<obstacle*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x-baseclass::coord.x,obstacles[i]->getRect()->y-baseclass::coord.y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				return true;
			}
		}
	}
	return false;
}

bool bullet::movCubeCollision(const std::vector<movingCube*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x-baseclass::coord.x,obstacles[i]->getRect()->y-baseclass::coord.y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				return true;
			}
		}
	}
	return false;
}

bool bullet::fallCubeCollision(const std::vector<fallingCube*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x-baseclass::coord.x,obstacles[i]->getRect()->y-baseclass::coord.y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				return true;
			}
		}
	}
	return false;
}

