#ifndef NETWORK_H
#define NETWORK_H
#include <SDL/SDL_net.h>
#include <iostream>
#include <cstring>
#include "player.h"
#include "otherPlayer.h"
#include "weapon.h"
#define SDL_reinterpret_cast(type, expression)reinterpret_cast<type>(expression)

class network{
	SDLNet_SocketSet server;
	TCPsocket connection;
	char tmp[1400];
	public:
	network(const char* c);
	~network();
	void send(player* p);
	void sendShot(player* p,int c);
	void recv(std::vector<otherPlayer*> &otherPlayers,std::vector<weapon*> weapons,player* p,std::vector<unsigned int> f);
};

#endif
