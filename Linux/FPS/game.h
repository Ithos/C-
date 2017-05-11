#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "text.h"
#include "vector3d.h"
#include "objLoader.h"
#include "cameraClass.h"
#include "collisionPlane.h"
#include "collisionSphere.h"
#include "level.h"
#include "player.h"
#include "weapon.h"
#include "enemy.h"
#include "item.h"
#include "network.h"
#include "otherPlayer.h"
#include "knife.h"
#include "grenade.h"
#include "particlesystem.h"

class game{
	bool online;//Esto indica si intentamos conectarnos al servidor o no
	network* net;
	
	objLoader obj;
	std::vector<level*> levels;
	std::vector<weapon*> weapons;
	std::vector<enemy*> enemies;
	std::vector<otherPlayer*> otherPlayers;
	std::vector<unsigned int> otherPlayersAnim;//Probablemente habría que cambiar esto
	std::vector<collisionPlane> bulletImpacts;//Planos de colision de las balas 
	unsigned int bulletImpactImg;//Imagen de los impactos
	item items;
	player* player1;
	
	particlesystem* ps;
	particlesystem* sparkles;
	vector3d sparklePos;
	
	void update();
	void show();
	unsigned int loadTexture(const char* filename, bool generate=true);
	void loadAnimation(std::vector<unsigned int>& frames,std::string filename,unsigned int num,std::vector<std::vector<collisionPlane> >* collplanes=NULL);
	
	public:
	game();
	~game();
	void start();
	
};

#endif
