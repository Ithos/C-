#include <SDL/SDL.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <SDL/SDL_thread.h>


int func(void *ptr){//Un puntero a void puede almacenar casi lo que sea, pero hay que hacer casts y cosas para usarlo
	
	while(1){
		std::cout<<"func "<< (int)ptr <<std::endl;
		SDL_Delay(rand()%10);
	}
}

int main (int argc, char** argv){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	float i=0.0;
	
	SDL_Thread* th1=SDL_CreateThread(func,(void*)1);//Esto crea y ejecuta el 1er thread
	SDL_Thread* th2=SDL_CreateThread(func,(void*)2);//Esto crea y ejecuta el 2º thread
	
	while(1){//El 3er thread es main
		i=sqrt(i);
		std::cout<<"main "<<i<<std::endl;
		i++;
		SDL_Delay(rand()%10);
	}
	/* Cuando lo ejecutamos vemos que el programa principal y los threads se intercalan de forma aleatoria*/
	SDL_Quit();
	return 0;
}
	
