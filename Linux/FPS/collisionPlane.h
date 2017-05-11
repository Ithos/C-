#ifndef COLLISIONPLANE_H
#define COLLISIONPLANE_H

#include "vector3d.h"

class collisionPlane{
	vector3d p[4];
	vector3d normal;
	int id;
	public:
	collisionPlane(float n1, float n2, float n3, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z, float p4x, float p4y, float p4z,int i=0);
	collisionPlane(const vector3d& n, const vector3d& p1, const vector3d& p2, const vector3d& p3, const vector3d& p4,int i=-1);
	vector3d getNormal();
	vector3d* getPoints();
	int getId();
};

#endif
