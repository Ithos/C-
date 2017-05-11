#include "player.h"

player::player(SDL_Surface *img){//En el constructor inicializamos cosas
	image=img;
	box.x=0;
	box.y=0;
	box.w=24;
	box.h=38;
	Xvel=Xvel_Abs=0;
	Yvel=Yvel_Abs=0;
	ground=0;
	jump=0;
	cubeFlag=0;
	fastFall=0;
	nJump=0;
	mossy=0;
	direction='r';
	frame=0.0;
	moving=false;
	onIce=false;
	onAir=false;
	Xcollision=false;
	health=10;
	for(int i=0;i<18;i++){
		clips[i].x=i*24;
		clips[i].y=0;
		clips[i].h=38;
		clips[i].w=24;
	}
}

player::~player(){
	SDL_FreeSurface(image);
}

SDL_Rect* player::getRect(){
	return &box;
}

void player::setXvel(int vel){
	Xvel=vel;
}

int player::getXvel(){
	return Xvel;
}
void player::setXvel_Abs(int vel){
	Xvel_Abs=vel;
}

int player::getXvel_Abs(){
	return Xvel_Abs;
}

void player::setYvel(int vel){
	Yvel=vel;
}

int player::getYvel(){
	return Yvel;
}
void player::setYvel_Abs(int vel){
	Yvel_Abs=vel;
}

int player::getYvel_Abs(){
	return Yvel_Abs;
}


void player::setX(int x){
	box.x=x;
}

void player::setY(int y){
	box.y=y;
}

void player::move(const std::vector<std::vector<int> >& map){//Aqui haremos la detección de colisiones

	bool grav=0;
	bool xcoll=0;

	int start=(baseclass::coord.x-(baseclass::coord.x%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Dividimos la pantalla en cuadros y recorremos solo los cuadros de la pantalla para optimizar
	int end=(baseclass::coord.x+baseclass::coord.w+(baseclass::TILE_SIZE-(baseclass::coord.x+baseclass::coord.w)%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Todo este codigo es para asegurar que los numeros que obtenemos son enteros
	
	
	if(start<0)start=0;//Esto es para que no leamos fuera de los vectores
	if(end>map[0].size())end=map[0].size();
	
	int yobj=box.y+Yvel;//Definimos las coordenadas a las que llegariamos sin colisiones
	int xobj=box.x+Xvel;
	
	for(int i=0;i<map.size();i++){
		for(int j=start;j<end;j++){
				
			if(map[i][j]!=0&&map[i][j]!=9){//El !=9 deberia ser solo un apaño temporal
					
					
					
				SDL_Rect destrect={j*50-baseclass::coord.x,i*50-baseclass::coord.y,50,50};
				
				if(Yvel_Abs>=0){
					int tmpy=binaryCollisionDown(&box,&destrect,Yvel);//Chequeamos colisiones hacia abajo
					if(tmpy<yobj&&tmpy>=box.y-5){//La funcion nos devuelve una posicion y chequeamos que esta no sea mas arriba que la inicial
						yobj=tmpy;
						ground=1;
						grav=1;
					}
				}else{
					int tmpy=binaryCollisionUp(&box,&destrect,-Yvel);
					if(tmpy>yobj&&tmpy){
						yobj=tmpy;
						grav=1;
					}
				}
				
				if(Xvel_Abs>=0){
					int tmpx=binaryCollisionRight(&box,&destrect,Xvel);
					if(tmpx<xobj){
						xobj=tmpx;
						xcoll=1;
					}
				}else{
					int tmpx=binaryCollisionLeft(&box,&destrect,-Xvel);
					if(tmpx>xobj){
						xobj=tmpx;
						xcoll=1;
					}
				}
				
				
			}	
		}
	}
	
			
	
	if (grav) Yvel=Yvel_Abs=0;
	if (xcoll) Xvel=Xvel_Abs=0;//Las velocidades y velocidades absolutas en la clase jugador deben mantenerse iguales, es la clase game la que decide si son distintas
	Xcollision=xcoll;
	if(Yvel!=0&&!cubeFlag){
		ground=0;//Esto es para evitar poder saltar en el aire si te caes de un bloque
	}
	if(cubeFlag)grav=1;
	
	if(ground){
		onAir=false;
		if(direction=='j')setDirection('r');
		if(direction=='k')setDirection('l');
		nJump=0;
		fastFall=0;
		Yvel=Yvel_Abs=0;
		jump=0;
	}else{
		onAir=true;
		if(direction=='r')setDirection('j');
		if(direction=='l')setDirection('k');
		if(!jump){
			nJump=1;
			jump=1;
		}
	}
	
	
	//los chequeos de gravedad y saltos deben estar fuera de los contadores para evitar que la velocidad del salto decrezca demasiado rapido
	if(!grav&&!jump){//Chequeamos si tenemos que aplicar gravedad
		if(nJump>2)Yvel=Yvel_Abs=10;
		else{
			if(Yvel<10){//Un truco un poco feo para no frenar el doble salto muy rapido
				Yvel++;
				Yvel_Abs++;
			}else{
				nJump++;
			}
		}
	}
	if(jump&&Yvel<10){
		if(fastFall){//Para caer mas rapido en el salto simple al soltar la tecla
			Yvel+=3;
			Yvel_Abs+=3;
		}else{
			Yvel++;//Si estamos saltando reducimos la velocidad de subida hasta alcanzar la de caida 
			Yvel_Abs++;
		}
	}/*else{
		jump=0;
	}*///Esto es de una version antigua sin doble salto
	
	
	box.y=yobj;
	box.x=xobj;
	
	
	if(moving){
		frame+=0.2;
		
		if(direction=='r'&&frame>=3.4)frame=0.0;
		if(direction=='l'&&frame>=7.4)frame=3.5;
		if(direction=='j'&&frame>=13.4)frame=13.4;
		if(direction=='k'&&frame>=17.4)frame=17.4;
		
	}else{
		
		if(direction=='r')frame=0.0;
		if(direction=='l')frame=3.5;
		if(direction=='m')frame=7.6;
		if(direction=='n')frame=8.6;
		
	}
	
	if(direction=='m'&&!mossy){
		if(ground)setDirection('r');
		else setDirection('j');
	}	
	if(direction=='n'&&!mossy){
		if(ground)setDirection('l');
		else setDirection('k');
	}
}

void player::setMoving(bool b){
	
	moving=b;
	
}

void player::setDirection(char c){
	
	if((c=='r'||c=='l')&& direction!=c){
		direction=c;
		if(!onAir){
			if(direction=='r')frame=0.0;
			else frame=3.6;
		}
	} else if(c=='m'||c=='n'){
		direction=c;
		if(direction=='m')frame=7.6;
		else frame=8.6;
	} else if((c=='j'||c=='k') && direction!=c){
		char d=direction;
		direction=c;
		if(direction=='j'&&d=='k')frame-=4.0;
		if(direction=='k'&&d=='j')frame+=4.0;
		if(direction=='j'&&(frame<9.5||frame>13.4))frame=9.5;
		else if(direction=='k'&&(frame<13.5||frame>17.4))frame=13.5;
	}
	
}
char player::getDirection(){
	return direction;
}

void player::setJump(int strenght){
	if(ground&&!jump){
		ground=0;
		jump=1;
		nJump++;
		Yvel=Yvel_Abs=-strenght;//Cambiamos la velocidad en y para subir
		box.y-=10;//Movemos el recuadro de dibujo un poco hacia arriba en cuanto saltamos
		if(direction=='j'||direction=='r')frame=9.5;
		if(direction=='k'||direction=='l')frame=13.5;
	}else{
		if(!ground&&(nJump<2)){
			fastFall=0;
			nJump++;
			if(mossy){
				if(direction=='m'){
					Xvel=Xvel_Abs=5;
					box.x+=5;
					setDirection('j');
					mossy=false;
				}else{
					Xvel=Xvel_Abs=-5;
					box.x-=5;
					setDirection('k');
					mossy=false;
				}
			}
			Yvel=Yvel_Abs=-10;//Cambiamos la velocidad en y para subir
			box.y-=5;//Movemos el recuadro de dibujo un poco hacia arriba en cuanto saltamos
			if(direction=='j'||direction=='r')frame=9.5;
			if(direction=='k'||direction=='l')frame=13.5;
		}
	}
}
void player::stopJump(){
	fastFall=1;
}
void player::setHealth(int h){
	
	health=h;
	
	}
int player::getHealth(){
	
	return health;
	
	}
bool player::getGround(){
	return ground;
}
void player::setGroundTrue(){
	ground=true;
		nJump=0;//Todo esto es para saltar alto cuando pisamos un enemigo
		jump=0;
		fastFall=0;
}

void player::setCFlag(bool b){
	cubeFlag=b;
}

void player::setSecondJump(){
	nJump=1;
}

bool player::getMossy(){
	return mossy;
}

void player::setMossy(bool moss){
	mossy=moss;
}

void player::setOnIce(bool ice){
	onIce=ice;
}

bool player::getOnIce(){
	return onIce;
}

bool player::getXcol(){
	return Xcollision;
}

void player::show(SDL_Surface *screen){
	SDL_BlitSurface(image,&clips[(int)(frame+0.5)],screen,&box);
}
