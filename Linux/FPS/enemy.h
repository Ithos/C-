#ifndef ENEMY_H
#define ENEMY_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "collision.h"
#include "collisionPlane.h"
#include "collisionSphere.h"
#include "matrix.h"
#include "particlesystem.h"

class enemy{
	std::vector<unsigned int> frames;
	unsigned int walk,attack,die;
	unsigned int curframe;
	
	int health,strength;
	float speed;
	vector3d direction,rotation;
	bool walking,attacking,dead;
	Uint32 dying;
	
	collisionSphere cs;

	public:
	enemy(std::vector<unsigned int>& anim,unsigned int w,unsigned int a,unsigned int d,int h,int str,float sp,collisionSphere css);
	~enemy();
	bool update(std::vector<collisionPlane>& col,vector3d playerPos);
	void show();
	bool setAttack(collisionSphere player);
	collisionSphere* getCollisionSphere();
	void setLocation(vector3d newLoc);
	void decreaseHealth(int num);
	int getHealth();
	int getStrength();
	bool isDead();
	bool disappear();
};

#endif
