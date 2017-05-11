#include "enemy.h"

enemy::enemy(SDL_Surface* img,int x,int y,int xvel,int yvel){//int dist
	image=img;
	box.y=y;//=Yini
	box.x=x;//=Xini
	box.w=box.h=30;
	Xvel=xvel;
	Yvel=yvel;
	//d=baseclass::TILE_SIZE*dist
	ground=0;
	jump=0;//teoricamente los enemigos no saltan pero dejo esto para implementarlo mas tarde
	frame=3.5;
	direction='r';//tendre que revisar un poco las direcciones
	jump=0;
	stomped=false;
	hit=false;
	del=false;
	counter=0;
	for(int i=0;i<11;i++){
		clips[i].x=i*30;
		clips[i].y=0;
		clips[i].w=clips[i].h=30;
	}
	
}

void enemy::show(SDL_Surface* screen){
	//Antes de mostrar a los enemigos tenemos que convertir las coordenadas absolutas a unas coordenadas relativas a nuestra pantalla
	
	SDL_Rect tmp={box.x-coord.x,box.y-coord.y,30,30};
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&tmp);
	
}

SDL_Rect* enemy::getRect(){
	
	return &box;
	
}
void enemy::enemyCollision(){
	Xvel=-Xvel;
	if(direction=='r')direction='l';
	else {
		direction='r';
		frame=3.5;
	}
	
}

void enemy::setHit(){
	hit=true;
	if(direction=='r')frame=9.5;
	else frame=8.5;
}

bool enemy::getHit(){
	return hit;
}

void enemy::setStomp(){
	stomped=true;
	frame=7.5;
}

bool enemy::getStomp(){
	return stomped;
}

bool enemy::getDeletion(){
	return del;
}

void enemy::increaseTimeCounter(){
	counter++;
	if(hit&&counter>2)del=true;
	if(stomped&&counter>14)del=true;
}

void enemy::move(const std::vector<std::vector<int> >& map){

	bool grav=0;
	//Baseclass:coord nos da problemas cuando el enemigo sale de la pantalla(creo)
	int start=(baseclass::coord.x-(baseclass::coord.x%baseclass::TILE_SIZE))/baseclass::TILE_SIZE-2;//Dividimos la pantalla en cuadros y recorremos solo los cuadros de la pantalla para optimizar
	int end=(baseclass::coord.x+baseclass::coord.w+(baseclass::TILE_SIZE-(baseclass::coord.x+baseclass::coord.w)%baseclass::TILE_SIZE))/baseclass::TILE_SIZE+2;//Todo este codigo es para asegurar que los numeros que obtenemos son enteros
	
	if(start<0)start=0;//Esto es para que no leamos fuera de los vectores
	if(end>map[0].size())end=map[0].size();
	
	int yobj=box.y+Yvel;//Definimos las coordenadas a las que llegariamos sin colisiones
	int xobj=box.x+Xvel;
	
	for(int i=0;i<map.size();i++){
		for(int j=start;j<end;j++){
				
			if(map[i][j]!=0){
					
					
					SDL_Rect destrect={j*50/*-baseclass::coord.x*/,i*50,50,50};//El comentario de dentro es para cambiar a coordenadas absolutas de coordenadas relativas
					
					if(Yvel>=0){
						int tmpy=binaryCollisionDown(&box,&destrect,Yvel);//Chequeamos colisiones hacia abajo
						if(tmpy<yobj&&tmpy>=box.y-5){//La funcion nos devuelve una posicion y chequeamos que esta no sea mas arriba que la inicial
							yobj=tmpy;
							ground=1;
							grav=1;
						}
					}else{
						int tmpy=binaryCollisionUp(&box,&destrect,-Yvel);
						if(tmpy>yobj&&tmpy<=box.y){
							yobj=tmpy;
							grav=1;
							Xvel=-Xvel;
							if(direction=='r')direction='l';
							else {
								direction='r';
								frame=3.5;
							}
						}
					}
					if(Xvel>=0){
						int tmpx=binaryCollisionRight(&box,&destrect,Xvel);
						if(tmpx<xobj){
							xobj=tmpx;
							Xvel=-Xvel;
							if(direction=='r')direction='l';
							else direction='r';
						}
					}else{
						int tmpx=binaryCollisionLeft(&box,&destrect,-Xvel);
						if(tmpx>xobj){
							xobj=tmpx;
							Xvel=-Xvel;
							if(direction=='r')direction='l';
							else {
								direction='r';
								frame=3.5;
							}
						}
					}
			}
		}
	}//los chequeos de gravedad y saltos deben estar fuera de los contadores para evitar que la velocidad del salto decrezca demasiado rapido
	
	if (grav==0) ground=0;//Para evitar que podamos saltar en el aire tras caer de algun lado
	
	if(!grav&&!jump){//Chequeamos si tenemos que aplicar gravedad
		Yvel=5;
	}
	if(jump&&Yvel<5){
		Yvel++;//Si estamos saltando reducimos la velocidad de subida hasta alcanzar la de caida 
	}else{
		jump=0;
	}
	
	
	box.y=yobj;
	box.x=xobj;
	
	frame+=0.2;//seleccionamos la animación segun la direccion
		
	if(direction=='l'&&frame>=3.4)frame=0.0;
	if(direction=='r'&&frame>=7.4)frame=3.5;
		
	
	
	
}

void enemy::obstacleCollision(const std::vector<obstacle*>& obstacles){
	if(box.x-baseclass::coord.x>-2*TILE_SIZE&&box.x-baseclass::coord.x<baseclass::coord.w+2*TILE_SIZE&&box.y-baseclass::coord.y>=0&&box.y-baseclass::coord.y<=baseclass::coord.h)//Si el enemigo esta en la pantalla
		for(int i=0;i<obstacles.size();i++){
			if(obstacles[i]->getRect()->x-baseclass::coord.x>-2*TILE_SIZE&&obstacles[i]->getRect()->x-baseclass::coord.x<=baseclass::coord.w+2*TILE_SIZE&&obstacles[i]->getRect()->y-baseclass::coord.y>=0&&obstacles[i]->getRect()->y-baseclass::coord.y<=baseclass::coord.h)
					if(collision(&box,obstacles[i]->getRect())){
						enemyCollision();
						if(box.x+box.w<=obstacles[i]->getRect()->x+obstacles[i]->getRect()->w/2)
							box.x=obstacles[i]->getRect()->x-box.w-1;//Evitamos que el enemigo se quede dentro del obstaculo
						else
							box.x=obstacles[i]->getRect()->x+obstacles[i]->getRect()->w+1;
					}	
		}
}




