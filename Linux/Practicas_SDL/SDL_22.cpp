#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <cstring>

int main (int argc, char** argv){

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	
	bool connection=true;
	
	IPaddress ip;//Esto es un struct que contendra lo que necesitamos para realizar la comunicacion a traves de ip
	SDLNet_ResolveHost(&ip,"127.0.0.1",1234);//Los parametros sion la ip a modificar, la direccion o ip a conectar (NULL para servidores) y el puerto a conectar
	
	TCPsocket client=SDLNet_TCP_Open(&ip);//Intentamos abrir una conexion con los datos almacenados en ip
	char text[100];
	SDLNet_TCP_Recv(client,text,100);
	std::cout<<text;
	
	SDLNet_TCP_Close(client);
	
	
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
