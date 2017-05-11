#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <iostream>
#include <vector>
#include <cstring>
#define SDL_reinterpret_cast(type, expression) reinterpret_cast<type>(expression) //Puede que esta macro no sea necesaria

struct data{
	TCPsocket socket;
	Uint32 timeout;
	int id;
	data(TCPsocket sock,Uint32 to,int i) : socket(sock),timeout(to),id(i){} //Creamos un constructor y llamamos a constructores delegados
};

int main(int argc,char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	int curid=0;//Ids de los jugadores
	int playernum=0;//Numero de jugadores conectados
	SDL_Event event;//Variable para manejar eventos
	IPaddress ip;//Ip del servidor
	SDLNet_ResolveHost(&ip,NULL,1234);//Nos conectamos como servidor al puerto 1234 y obtenemos una ip
	std::vector<data> socketvector;//en este vector almacenamos las conexiones de los clientes
	char tmp[1400];
	bool run=true;
	SDLNet_SocketSet sockets=SDLNet_AllocSocketSet(30);//Esto almacena varios sockets para poder recibir datos de cada uno sin tener que bloquear el programaç
	SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);//Creamos una ventana ya que si no el servidor no responde a eventos, que en este caso el evento Quit
	TCPsocket server=SDLNet_TCP_Open(&ip);//Este es el socket para el servidor
	
	while(run){
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT || event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE){
				run=false;
			}
		}
			TCPsocket tmpsocket=SDLNet_TCP_Accept(server);
			if(tmpsocket){
				if(playernum<30){
					SDLNet_TCP_AddSocket(sockets,tmpsocket);//Añadimos tmpsocket a sockets
					socketvector.push_back(data(tmpsocket,SDL_GetTicks(),curid));//Añadimos el socket al vector
					playernum++;
					sprintf(tmp,"0 %d \n",curid);
					curid++;
					std::cout<<"New connection:"<<curid<<std::endl;
				}else{
					sprintf(tmp,"3 \n"); //Damos el mensaje de que el servidor esta lleno
				}
				
				std::cout<<"send response"<<std::endl;
				SDLNet_TCP_Send(tmpsocket,tmp,strlen(tmp)+1);//Mandamos el mensaje
				
			}
			while(SDLNet_CheckSockets(sockets,0)>0){//Comprobamos los mensajes recibidos
				for(int i=0;i<socketvector.size();i++){
					if(SDLNet_SocketReady(socketvector[i].socket)){
						socketvector[i].timeout=SDL_GetTicks();
						SDLNet_TCP_Recv(socketvector[i].socket,tmp,1400);//Almacenamos los mensajes en tmp
						int num=tmp[0]-'0';
						int j=1;
						while(tmp[j]>='0'&&tmp[j]<='9'){//Leemos numeros
								num*=10;
								num+=tmp[j]-'0';
								j++;
						}
						//Aqui determinamos el tipo de mensje
						if(num==1){//Actualizacion de la posicion
							for(int k=0;k<socketvector.size();k++){
								if(k!=i){
									SDLNet_TCP_Send(socketvector[k].socket,tmp,strlen(tmp)+1);
								}
							}
						}else if(num==2){//Jugador desconectado
							for(int k=0;k<socketvector.size();k++){//Mandamos mensajes para notificar
								if(k!=i){
									SDLNet_TCP_Send(socketvector[k].socket,tmp,strlen(tmp)+1);
								}
							}
							//Desconectamos al jugador
							SDLNet_TCP_DelSocket(sockets,socketvector[i].socket);
							SDLNet_TCP_Close(socketvector[i].socket);
							socketvector.erase(socketvector.begin()+i);
							playernum--;
							std::cout<<"player disconnected"<<std::endl;
						}else if(num==3){//Un jugador ha disparado a otro
							int tmpvar;
							sscanf(tmp,"3 %d",&tmpvar);
							for(int k=0;k<socketvector.size();k++){
								if(socketvector[k].id==tmpvar){//Enviamos el mensaje al otro jugador y salimos del bucle
									SDLNet_TCP_Send(socketvector[k].socket,tmp,strlen(tmp)+1);
									break;
								}
							}
						}
						
						
						
					}
				}
			}
		
		//Checkeamos timeouts
						
		for(int l=0;l<socketvector.size();l++){
			if(SDL_GetTicks()-socketvector[l].timeout>5000){
				sprintf(tmp,"2 %d \n",socketvector[l].id);//Creamos un mensaje que enviar
				for(int k=0;k<socketvector.size();k++){
					SDLNet_TCP_Send(socketvector[k].socket,tmp,strlen(tmp)+1);//Mandamos el mensaje también al jugador al que desconectamos
				}
					SDLNet_TCP_DelSocket(sockets,socketvector[l].socket);
					SDLNet_TCP_Close(socketvector[l].socket);
					socketvector.erase(socketvector.begin()+l);
					playernum--;
			}
		}
		
		SDL_Delay(1);//Añadimos un tiempo de espera para no tenere el procesador ocupado al 100%
		
	}
	
	for(int i=0;i<socketvector.size();i++){
		SDLNet_TCP_Close(socketvector[i].socket);
	}
	
	SDLNet_FreeSocketSet(sockets);
	SDLNet_TCP_Close(server);
	SDLNet_Quit();
	SDL_Quit();
}
