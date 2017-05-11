#include "grenade.h"

grenade::grenade(unsigned int exp,float rad,int ti,int d,unsigned int fireTex,unsigned int smokeTex){
	active=false;
	exploding=false;
	ground=false;
	t=100;
	damage=d;
	mesh=exp;
	r=rad;
	smoke=new particlesystem(300,vector3d(0,0,0),0.1,0.5,1.0,5.0,vector3d(0,0,0),vector3d(255,150,150),vector3d(255,255,255),1500,4000,0,smokeTex);
	fire=new particlesystem(50,vector3d(0,0,0),0.01,0.05,3.0,10.0,vector3d(0,0,0),vector3d(255,255,255),vector3d(255,255,255),500,2000,0,fireTex);
}

grenade::~grenade(){
	delete smoke;
    delete fire;
}

bool grenade::update(std::vector<enemy*>& enemies,std::vector<collisionPlane>& planes){
	if(active && !exploding){
		t--;
		if(t<=0){
			exploding=true;
			//smoke->setPosition(position);
			smoke->setNumParticles(300);
			//fire->setPosition(position);
			fire->setNumParticles(100);
			for(int i=0;i<enemies.size();i++){
				collisionSphere tmpCS(*enemies[i]->getCollisionSphere());
				if(collision::spheresphere(tmpCS.getCenter(),tmpCS.getR(),pos,30.0)){//Esto nos indica si el enemigo esta dentro del área de la explosión
					//Trazamos una recta desde la posicion de la granada hasta el enemigo y comprobamos la distancia para calcular el daño
					vector3d vec=pos-enemies[i]->getCollisionSphere()->getCenter();
					enemies[i]->decreaseHealth((30.0/vec.lenght())*damage);
				}
			}
			return true;
		}
		if(!ground){
			vector3d falling(0.0,0.2,0.0);
			force-=falling;
			int numchecks=20;
			for(int j=0;j<numchecks;j++){//Actualizamos la posición en varios pasos
				pos+=force/(float)numchecks;
				for(int i=0;i<planes.size();i++){//Comprobamos colisiones con planos de colisión
					if(collision::sphereplane(pos,planes[i].getNormal(),planes[i].getPoints()[0],planes[i].getPoints()[1],planes[i].getPoints()[2],planes[i].getPoints()[3],r)){
						ground=true; //Debería comprobar si las granadas rebotan contra las paredes
						return true;
					}
				}
			}
		}
	}else if(exploding){
		smoke->update();
		fire->update();
		if(!smoke->isParticleSystemActive() && !fire->isParticleSystemActive())active=false;//Cuando terminen los efectos la granada deja de estar activa
	}
	return false;
}

void grenade::show(){
	if(active){
		glPushMatrix();
			glTranslatef(pos.getX(),pos.getY(),pos.getZ());//Simplemente representamos el modelo en la posición en la que se encuentre
			glCallList(mesh);
		glPopMatrix();
	}
}

vector3d grenade::getPosition(){
	return pos;
}

void grenade::dropGrenade(vector3d direction,const vector3d& location,float mult){
	active=true;
	force=direction*mult;//Mult es un parámetro que indica la fuerza con la que se lanza la granada
	pos=location;
}

float grenade::getDamage(){
	return damage;
}

void grenade::showParticleSystem(camera& cam){
	smoke->show(cam);
	fire->show(cam);
}
