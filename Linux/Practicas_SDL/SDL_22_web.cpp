#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <cstring>

int main (int argc, char** argv){

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	
	bool connection=true;
	
	IPaddress ip;//Esto es un struct que contendra lo que necesitamos para realizar la comunicacion a traves de ip
	SDLNet_ResolveHost(&ip,"www.linux.org",80);//Los parametros sion la ip a modificar, la direccion o ip a conectar (NULL para servidores) y el puerto a conectar
	
	const char *http="GET / HTTP/1.1\nHost:www.linux.org\n\n";//Con esto deberiamos obtener el codigo fuente de la pagina
	
	TCPsocket client=SDLNet_TCP_Open(&ip);//Intentamos abrir una conexion con los datos almacenados en ip
	
	SDLNet_TCP_Send(client,http,strlen(http)+1);//Mandamos la peticion
	
	char text[10000];
	while(SDLNet_TCP_Recv(client,text,10000))std::cout<<text;//mientras recibamos datos los mostramos por pantalla
	//Recv bloquea la ejecucion hasta que recibamos algo, por lo que habrá que matar el proceso (a menos que tenga un timeout)
	
	SDLNet_TCP_Close(client);
	
	
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
