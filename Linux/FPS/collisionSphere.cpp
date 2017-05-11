#include "collisionSphere.h"

collisionSphere::collisionSphere(){
	r=0;//Solo inicializamos el radio, el constructor por defecto de vector3d se encargará de inicializa center
}

collisionSphere::collisionSphere(const vector3d& vec,float rad){
	center.change(vec);
	r=rad;
}

float collisionSphere::getR(){
	return r;
}

void collisionSphere::setR(float rad){
	r=rad;
}

vector3d& collisionSphere::getCenter(){
	return center;
}
void collisionSphere::setCenter(const vector3d& vec){
	center.change(vec);
}
