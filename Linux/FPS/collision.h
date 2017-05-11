#ifndef COLLISION_H
#define COLLISION_H

#include <stdlib.h>
#include "vector3d.h"

class collision{
	public:
		static bool raysphere(float xc,float yc,float zc,float xd,float yd,float zd,float xs,float ys,float zs,float r, float* dist=NULL, vector3d* point=NULL);
		static bool rayplane(float xn,float yn,float zn,float xs,float ys,float zs,float xd,float yd,float zd,const vector3d& p1,const vector3d& p2,const vector3d& p3,const vector3d& p4, float* dist=NULL, vector3d* point=NULL);
		static float trianglearea(const vector3d& p1,const vector3d& p2,const vector3d& p3);
		static bool sphereplane(vector3d &sp, const vector3d& vn,const vector3d& p1,const vector3d& p2,const vector3d& p3,const vector3d& p4,float r,bool up=false,bool* ramp=NULL);
		static bool spheresphere(vector3d &c1,float r1,const vector3d& c2,float r2);
		static float pointdistance(const vector3d& p1,const vector3d& p2);
};

#endif
