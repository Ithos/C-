#include "mine.h"

mine::mine(SDL_Surface* img,int x,int y){
	box.x=x;
	box.y=y;
	box.h=26;
	box.w=26;
	velx=0;
	vely=0;
	image=img;
	hitFlag=false;
	frame=0.0;
	
	clips[0].x=0;
	clips[0].y=0;
	clips[0].h=26;
	clips[0].w=26;
	
	clips[1].x=40;
	clips[1].y=0;
	clips[1].h=20;
	clips[1].w=20;
	
	for(int i=2;i<5;i++){
		clips[i].x=i*40;
		clips[i].y=0;
		clips[i].h=40;
		clips[i].w=40;
	}
	
	srand (time(NULL));
	
}

void mine::show(SDL_Surface* screen){
	SDL_Rect tmp={box.x-coord.x,box.y-coord.y,box.w,box.h};//Convertimos a coordenadas relativas a la pantalla
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&tmp);//Bliteamos
}

SDL_Rect* mine::getRect(){
	return &box;
}

void mine::hit(){
	hitFlag=true;
}

bool mine::getHit(){
	return hitFlag;
}

bool mine::move(int playerX,int playerY,const std::vector<std::vector<int> >& map,std::vector<obstacle*> obstacles,std::vector<movingCube*> movCubes,std::vector<fallingCube*> fallCubes){
	if(!hitFlag&&(playerX+coord.x>=box.x-5*baseclass::TILE_SIZE&&playerX+coord.x<=box.x+box.w+5*baseclass::TILE_SIZE&&playerY+coord.y>=box.y-5*baseclass::TILE_SIZE&&playerY+coord.y<=box.y+box.h+5*baseclass::TILE_SIZE)){
		
		if(frame<0.5){
				frame=0.8;
				box.w=20;
				box.h=20;
				box.x-=3;
				box.y+=3;
			}
		
		if(playerX+coord.x>=box.x-5&&playerX+coord.x<=box.x+box.w+5&&playerY+coord.y>=box.y-5&&playerY+coord.y<=box.y+box.h+5){
			hitFlag=true;
		}
		
		if(playerX+coord.x<box.x&&velx>-6)velx--;
		if(playerX+coord.x>box.x&&velx<6)velx++;
		if(playerY+coord.y<box.y&&vely>-6)vely--;
		if(playerY+coord.y>box.y&&vely<6)vely++;
		
		int tmpx=box.x,tmpy=box.y;
		
		box.x=box.x+velx+rand()%2-rand()%2;
		box.y=box.y+vely+rand()%2-rand()%2;
		
		if(mapCollision(map)||obstacleCollision(obstacles)||movCubeCollision(movCubes)||fallCubeCollision(fallCubes)){
			box.x=tmpx;
			box.y=tmpy;
			movCubeCollision(movCubes);
		}
		
		return false;
		
	}else{
		if(hitFlag){
			if(frame<0.5){
				frame=0.8;
				box.w=20;
				box.h=20;
				box.x-=3;
				box.y+=3;
			}
			bool changeFrame=frame<1.5;
			frame+=0.3;
			if(changeFrame&&frame>=1.5){
				box.w=20;
				box.h=20;
				box.x+=10;
				box.y-=10;
			}
			if(frame>=4.3)return true;
			else return false;
		}
		
		bool changeFrame=frame>=0.5;
		frame=0.0;//Cambiamos de imagen
		if(changeFrame){
		box.w=26;
		box.h=26;
		box.x+=3;
		box.y-=3;
		}
		return false;
	}
}

bool mine::mapCollision(const std::vector<std::vector<int> >& map){
	//Checkeamos choques contra el mapa
	int srt=(baseclass::coord.x-(baseclass::coord.x%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Dividimos la pantalla en cuadros y recorremos solo los cuadros de la pantalla para optimizar
	int end=(baseclass::coord.x+baseclass::coord.w+(baseclass::TILE_SIZE-(baseclass::coord.x+baseclass::coord.w)%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Todo este codigo es para asegurar que los numeros que obtenemos son enteros
	
	if(srt<0)srt=0;//Esto es para que no leamos fuera de los vectores
	if(end>map[0].size())end=map[0].size();
	
	for(int i=0;i<map.size();i++){//Conviene revisar las colisiones
		for(int j=srt;j<end;j++){
				
			if(map[i][j]!=0){
					
					SDL_Rect destrect={j*50,i*50,50,50};
						if(collision(&destrect,&box)){
							return true;
						
						}
					
				}
				
			}
			
		}
		return false;
}

bool mine::obstacleCollision(const std::vector<obstacle*>& obstacles){
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

bool mine::movCubeCollision(const std::vector<movingCube*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x,obstacles[i]->getRect()->y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				if(obstacles[i]->getYvel()!=0){
					if(obstacles[i]->getYvel()>0&&box.y>=obstacles[i]->getRect()->y){
						while(collision(&box,&destrect)){
							box.y++;
							if(box.x+box.w/2>=obstacles[i]->getRect()->x+obstacles[i]->getRect()->w/2)box.x++;
							else box.x--;
						}
					}
					if(obstacles[i]->getYvel()<0&&box.y<=obstacles[i]->getRect()->y){
						while(collision(&box,&destrect)){
							box.y--;
							if(box.x+box.w/2>=obstacles[i]->getRect()->x+obstacles[i]->getRect()->w/2)box.x++;
							else box.x--;
						}
					}
				}else{
					if(obstacles[i]->getXvel()>0&&box.x>=obstacles[i]->getRect()->x){
						while(collision(&box,&destrect)){
							box.x++;
							if(box.y+box.h/2>=obstacles[i]->getRect()->y+obstacles[i]->getRect()->h/2)box.y++;
							else box.y--;
						}
					}
					if(obstacles[i]->getXvel()<0&&box.x<=obstacles[i]->getRect()->x){
						while(collision(&box,&destrect)){
							box.x--;
							if(box.y+box.h/2>=obstacles[i]->getRect()->y+obstacles[i]->getRect()->h/2)box.y++;
							else box.y--;
						}
					}
				}
				return true;
			}
		}
	}
	return false;
}

bool mine::fallCubeCollision(const std::vector<fallingCube*>& obstacles){
	for(int i=0;i<obstacles.size();i++){
		if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h){//Si el obstaculo esta en la pantalla +- 2 cuadros
			SDL_Rect destrect={obstacles[i]->getRect()->x,obstacles[i]->getRect()->y,obstacles[i]->getRect()->w,obstacles[i]->getRect()->h};
			if(collision(&box,&destrect)){
				if(!obstacles[i]->getFalling())return true;
			}
		}
	}
	return false;
}

