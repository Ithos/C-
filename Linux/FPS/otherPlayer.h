#ifndef OTHERPLAYER_H
#define OTHERPLAYER_H

#include <vector>
#include <GL/gl.h>
#include <iostream>
#include "collisionSphere.h"
#include "vector3d.h"
#include "weapon.h"

struct otherPlayer{
	int id;
	std::vector<unsigned int> frames;
	collisionSphere cs;
	int curframe;
	vector3d position;
	vector3d rotation;
	vector3d lookDirection;
	int health;
	weapon* w;
	otherPlayer(std::vector<unsigned int> f,int i);
	void show();
};

#endif
