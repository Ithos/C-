#include "collisionPlane.h"

collisionPlane::collisionPlane(float n1, float n2, float n3, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z, float p4x, float p4y, float p4z,int i){
	normal.change(n1,n2,n3);
	p[0].change(p1x,p1y,p1z);
	p[1].change(p2x,p2y,p2z);
	p[2].change(p3x,p3y,p3z);
	p[3].change(p4x,p4y,p4z);
	id=i;
}

collisionPlane::collisionPlane(const vector3d& n, const vector3d& p1, const vector3d& p2, const vector3d& p3, const vector3d& p4,int i){
	normal.change(n);
	p[0].change(p1);
	p[1].change(p2);
	p[2].change(p3);
	p[3].change(p4);
	id=i;
}

vector3d collisionPlane::getNormal(){
	return normal;
}

vector3d* collisionPlane::getPoints(){
	return p;
}

int collisionPlane::getId(){
	return id;
}
