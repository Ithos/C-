#include "base.h"

bool baseclass::collision(SDL_Rect *rec1, SDL_Rect *rec2){//Chequeamos cuando no hay colisión y devolvemos 0, por defecto devolvemos 1, es decir damos colisión
	if(rec1->y >= rec2->y + rec2->h)return 0;
	if(rec1->x >= rec2->x + rec2->w)return 0;
	if(rec1->y + rec1->h <= rec2->y)return 0;
	if(rec1->x + rec1->w <= rec2->x)return 0;
	return 1;
	
}

int baseclass::binaryCollisionUp(SDL_Rect* rect1, SDL_Rect* rect2 ,int yvel){
	
	//Creamos un rect que se extienda desde la posicion original hasta la posicion final, chequeamos colisiones y si se dan lo dividimos en dos
	//chequeamos de nuevo y continuamos hasta detectar el punto de colision, despues damos la posicion objetivo
	//eliminamos la posibilidad de h=0 para eliminar las colisiones que no se dan en la direccion que chequeamos
	
	SDL_Rect tmp1={rect1->x,rect1->y-yvel,rect1->w,rect1->h+yvel};
			if(collision(&tmp1,rect2)){
				SDL_Rect tmp2={tmp1.x,tmp1.y+tmp1.h/2,tmp1.w,tmp1.h/2};
				tmp1.h=tmp1.h/2;
				while(!(tmp1.y<=rect2->y+rect2->h&&tmp1.y>=rect2->y+rect2->h-5)&&!(tmp2.y<=rect2->y+rect2->h&&tmp2.y>=rect2->y+rect2->h-5)&&tmp1.h!=0&&tmp2.h!=0){
					if(collision(&tmp2,rect2)){
						tmp1.y=tmp2.y;
						tmp1.h=tmp2.h/2;
						tmp2.y=tmp2.y+tmp2.h/2;
						tmp2.h=tmp2.h/2;
					}else if(collision(&tmp1,rect2)){
						tmp2.y=tmp1.y+tmp1.h/2;
						tmp2.h=tmp2.h/2;
						tmp1.h=tmp1.h/2;
					}else{
						tmp1.h=0;
						tmp2.h=0;
					}
				}
				if(tmp1.h==0||tmp2.h==0)return rect1->y-yvel;
				else if(tmp1.y<=rect2->y+rect2->h&&tmp1.y>=rect2->y+rect2->h-5)return rect2->y+rect2->h;//tmp1.y;
				else if(tmp2.y<=rect2->y+rect2->h&&tmp2.y>=rect2->y+rect2->h-5)return rect2->y+rect2->h;//tmp2.y;	//Esto ultimo es poco elegante, igual lo cambio en versiones posteriores
			}else
			return rect1->y-yvel;
	
	
}

int baseclass::binaryCollisionDown(SDL_Rect* rect1,SDL_Rect* rect2,int yvel){
	
	SDL_Rect tmp1={rect1->x,rect1->y,rect1->w,rect1->h+yvel};
			if(collision(&tmp1,rect2)){
				SDL_Rect tmp2={tmp1.x,tmp1.y+tmp1.h/2,tmp1.w,tmp1.h/2};
				tmp1.h=tmp1.h/2;
				while(!((tmp1.y+tmp1.h>=rect2->y)&&(tmp1.y+tmp1.h<=rect2->y+5))&&!((tmp2.y+tmp2.h>=rect2->y)&&(tmp2.y+tmp2.h<=rect2->y+5))&&tmp1.h!=0&&tmp2.h!=0){
					if(collision(&tmp1,rect2)){
						tmp2.y=tmp1.y+tmp1.h/2;
						tmp2.h=tmp2.h/2;
						tmp1.h=tmp1.h/2;
					}else if(collision(&tmp2,rect2)){
						tmp1.y=tmp2.y;
						tmp1.h=tmp2.h/2;
						tmp2.y=tmp2.y+tmp2.h/2;
						tmp2.h=tmp2.h/2;
					}else{
						tmp1.h=0;
						tmp2.h=0;
					}
				}
				if(tmp1.h==0||tmp2.h==0)return rect1->y+yvel;
				else if(tmp1.y+tmp1.h>=rect2->y&&tmp1.y+tmp1.h<=rect2->y+5)return rect2->y-rect1->h;//rect1.y=tmp1.y+tmp1.h-rect1->h;
				else if(tmp2.y+tmp2.h>=rect2->y&&tmp2.y+tmp2.h<=rect2->y+5)return rect2->y-rect1->h;//rect1.y=tmp2.y+tmp2.h-rect1->h;	
			}else
			return rect1->y+yvel;
	
	
}

int baseclass::binaryCollisionRight(SDL_Rect* rect1,SDL_Rect* rect2,int xvel){
	SDL_Rect tmp1={rect1->x,rect1->y,rect1->w+xvel,rect1->h};
			if(collision(&tmp1,rect2)){
				SDL_Rect tmp2={tmp1.x+tmp1.w/2,tmp1.y,tmp1.w/2,tmp1.h};
				tmp1.w=tmp1.w/2;
				while(!(tmp1.x+tmp1.w>=rect2->x&&tmp1.x+tmp1.w<=rect2->x+5)&&!(tmp2.x+tmp2.w>=rect2->x&&tmp2.x+tmp2.w<=rect2->x+5)&&tmp1.w!=0&&tmp2.w!=0){
					if(collision(&tmp1,rect2)){
						tmp2.x=tmp1.x+tmp1.w/2;
						tmp2.w=tmp2.w/2;
						tmp1.w=tmp1.w/2;
					}else if(collision(&tmp2,rect2)){
						tmp1.x=tmp2.x;
						tmp1.w=tmp2.w/2;
						tmp2.x=tmp2.x+tmp2.w/2;
						tmp2.w=tmp2.w/2;
					}else{
						tmp1.w=0;
						tmp2.w=0;
					}
				}
				if(tmp1.w==0||tmp2.w==0)return rect1->x+xvel;
				else if(tmp1.x+tmp1.w>=rect2->x&&tmp1.x+tmp1.w<=rect2->x+5)return rect2->x-rect1->w;//rect1->x=tmp1.x+tmp1.w-rect1->w;
				else if(tmp2.x+tmp2.x>=rect2->x&&tmp2.x+tmp2.w<=rect2->x+5)return rect2->x-rect1->w;//rect1->x=tmp2.x+tmp1.w-rect1->w;	
			}else
			return rect1->x+xvel;
}

int baseclass::binaryCollisionLeft(SDL_Rect* rect1,SDL_Rect* rect2,int xvel){
	
	SDL_Rect tmp1={rect1->x-xvel,rect1->y,rect1->w+xvel,rect1->h};
			if(collision(&tmp1,rect2)){
				SDL_Rect tmp2={tmp1.x+tmp1.w/2,tmp1.y,tmp1.w/2,tmp1.h};
				tmp1.w=tmp1.w/2;
				while(!(tmp1.x<=rect2->x+rect2->w&&tmp1.x>=rect2->x+rect2->w-5)&&!(tmp2.x<=rect2->x+rect2->w&&tmp2.x>=rect2->x+rect2->w-5)&&tmp1.w!=0&&tmp2.w!=0){
					if(collision(&tmp2,rect2)){
						tmp1.x=tmp2.x;
						tmp1.w=tmp2.w/2;
						tmp2.x=tmp2.x+tmp2.w/2;
						tmp2.w=tmp2.w/2;
					}else if(collision(&tmp1,rect2)){
						tmp2.x=tmp1.x+tmp1.w/2;
						tmp2.w=tmp2.w/2;
						tmp1.w=tmp1.w/2;
					}else{
						tmp1.w=0;
						tmp2.w=0;
					}
				}
				if(tmp1.w==0||tmp2.w==0)return rect1->x-xvel;
				else if(tmp1.x<=rect2->x+rect2->w&&tmp1.x>=rect2->x+rect2->w-5)return rect2->x+rect2->w;//rect1->x=tmp1.x;
				else if(tmp2.x<=rect2->x+rect2->w&&tmp2.x>=rect2->x+rect2->w-5)return rect2->x+rect2->w;//rect1.x=tmp2.x;	
			}else
			return rect1->x-xvel;
		}
