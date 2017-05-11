#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <cstring>

int main (int argc, char** argv){

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	
	bool connection=true;
	
	IPaddress ip;//Esto es un struct que contendra lo que necesitamos para realizar la comunicacion a traves de ip
	SDLNet_ResolveHost(&ip,NULL,1234);//Los parametros sion la ip a modificar, la direccion o ip a conectar (NULL para servidores) y el puerto a conectar
	
	
	TCPsocket server=SDLNet_TCP_Open(&ip);//Es basicamente un listen le decimos de donde tiene que escuchar, la ejecucion no se detiene hasta que recibimos una conexion
	TCPsocket client;
	const char *text="Hello\n";
	while(connection){//Esperamos la conexion en el while
		client=SDLNet_TCP_Accept(server);//Creamos un socket para el cliente y le decimos el socket servidor
		if(client){//Esto solo se ejecuta si hay un cliente
			//Mandamos un mensaje y cerramos (pero podriamos hacer mas cosas(mirar el de python))
			SDLNet_TCP_Send(client,text,strlen(text)+1);//El +1 es por el caracter de fin de string
			SDLNet_TCP_Close(client);
			connection=false;
		}
		
	}
	SDLNet_TCP_Close(server);
	
	
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
