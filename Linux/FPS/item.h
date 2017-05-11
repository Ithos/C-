#ifndef ITEM_H
#define ITEM_H

#include <GL/gl.h>
#include <iostream>
#include <vector>
#include "collision.h"
#include "collisionSphere.h"
#include "vector3d.h"

class specItem{
	vector3d rotation,scale;
	collisionSphere cs;
	int id;
	unsigned int mesh;
	
	public:
	specItem(const vector3d& rot,const vector3d& sc,const collisionSphere& css,int idn,unsigned int m); 
	int getId();
	vector3d getRot();
	vector3d getScale();
	void setRot(const vector3d& rot);
	collisionSphere getPos();
	unsigned int getMesh();
};

class item{
	std::vector<specItem> items;
	
	public:
	void add(const vector3d& rot,const vector3d& sc,const collisionSphere& css,int idn,unsigned int m);
	void del(int id);
	int update(collisionSphere player);
	void show();
};

#endif
