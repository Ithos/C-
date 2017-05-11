#include <SDL/SDL.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <SDL/SDL_thread.h>

SDL_mutex* mut;

int func(void *ptr){//Un puntero a void puede almacenar casi lo que sea, pero hay que hacer casts y cosas para usarlo
	
	SDL_LockMutex(mut);//Esperamos a que mut se abra para continuar y lo cerramos
	float* var=(float*)ptr;
	for(int i=1;i<11;i++){
		std::cout<<"sumamos "<< i <<std::endl;
		(*var)+=i;
		SDL_Delay(rand()%10);
	}
	SDL_UnlockMutex(mut);
}

int main (int argc, char** argv){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	float i=0.0;
	
	float var=1.0;
	
	
	
	while(1){//El 3er thread es main
		SDL_Thread* th=SDL_CreateThread(func,(void*)&var);//Esto crea y ejecuta el thread en cada ciclo
		SDL_Delay(100);//Nos aseguramos de que se ejecute primero el thread
		SDL_LockMutex(mut);//Si mut esta cerrado detiene la ejecucion hasta que se abra, luego lo cierra y continua
		for(int i=1;i<11;i++){
			std::cout<<"restamos "<< i <<std::endl;
			var-=i;
			SDL_Delay(rand()%10);
		}
		
		std::cout<< var <<std::endl;
		var=1.0;
		SDL_UnlockMutex(mut);//Abrimos mut cuando terminamos de cambiar variables comunes
		SDL_KillThread(th);//Matamos el thread para que no cambie var mas en este ciclo de ejecucion
		SDL_Delay(rand()%10);
		
	}
	//Este programa esta para ver la diferencia entre usar un sistema de sincronizacion de threads y no usarlo
	//Creo que ubuntu sincroniza los threads automaticamente
	SDL_Quit();
	return 0;
}
	
