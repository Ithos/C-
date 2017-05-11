#ifndef GRENADE_H
#define GRENADE_H
#include <vector>
#include <GL/gl.h>
#include "vector3d.h"
#include "collision.h"
#include "enemy.h"
#include "collisionPlane.h"
#include "cameraClass.h"
#include "particlesystem.h"

class grenade{
	bool active,exploding,ground;
	unsigned int mesh;
	vector3d force;
	vector3d pos;
	int t;
	int damage;
	float r;
	particlesystem* smoke;
	particlesystem* fire;
	public:
	grenade(unsigned int exp,float rad,int ti,int d,unsigned int fireTex,unsigned int smokeTex);
	~grenade();
	bool update(std::vector<enemy*>& enemies,std::vector<collisionPlane>& planes);
	void show();
	vector3d getPosition();
	void dropGrenade(vector3d direction,const vector3d& location,float mult);
	float getDamage();
	void showParticleSystem(camera& cam);//Separamos la representación de las parículas, ya que tenemos que representarlas las últimas
	
};

#endif
