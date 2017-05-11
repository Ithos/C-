#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H

#include "vector3d.h"

class collisionSphere{
	float r;
	vector3d center;
	public:
	collisionSphere();
	collisionSphere(const vector3d& vec,float rad);
	float getR();
	void setR(float rad);
	vector3d& getCenter();
	void setCenter(const vector3d& vec);
};

#endif
