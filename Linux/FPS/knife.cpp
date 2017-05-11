#include "knife.h"

knife::knife(std::vector<unsigned int>& f,int dmg,int del,int wdel,const vector3d& location){
	damage=dmg;
	delay=del;
	wepDelay=wdel;
	inUse=false;
	curframe=0;
	lastUse=100;
	loc=location;
	frames=f;
	testing=false;
}

int knife::setKnife(std::vector<enemy*>& enemies,const vector3d& dir,const vector3d& loc){
	if(!inUse&&lastUse>delay){
		lastUse=0;
		inUse=true;
		curframe=0;
		float dist;
		for(int i=0;i<enemies.size();i++){
			if(collision::raysphere(enemies[i]->getCollisionSphere()->getCenter().getX(),enemies[i]->getCollisionSphere()->getCenter().getY(),enemies[i]->getCollisionSphere()->getCenter().getZ(),dir.getX(),dir.getY(),dir.getZ(),loc.getX(),loc.getY(),loc.getZ(),enemies[i]->getCollisionSphere()->getR(),&dist)&& -dist<5.0 && -dist>=0.0 && !enemies[i]->isDead()){
				std::cout << "knife dist" << -dist << std::endl;
					enemies[i]->decreaseHealth(damage);
					return i;//Devolvemos la id del enemigo impactado
				//Habría que mirar como adaptar este modelo para que afecte a otros jugadores en multijugador
			}
		}
	}
	return -1;//Si no se impacta por alguna razón damos -1
}

bool knife::update(){
	if(testing)test();
	lastUse++;
	if(lastUse>wepDelay)return false;
	if(inUse){
		curframe++;
		if(curframe>=frames.size())
		{
			inUse=false;
			curframe=0;
		}
	}
	return true;
}

void knife::show(){
	if(inUse){
		glPushMatrix();
			glLoadIdentity();
			glTranslatef(loc.getX(),loc.getY(),loc.getZ());
			glScalef(1.0,1.0,1.0);
			glCallList(frames[curframe]);//Voy a usar la animación, aunque podría cargar un modelo y rotarlo
		glPopMatrix();
	}
}

bool knife::isUsing(){
	return inUse;
}

int knife::getDamage(){
	return damage;
}

void knife::test(){
	Uint8* keys=SDL_GetKeyState(NULL);
		if(keys[SDLK_j])
			loc.setX(loc.getX()-0.1);
		if(keys[SDLK_l])
			loc.setX(loc.getX()+0.1);
		if(keys[SDLK_k])
			loc.setZ(loc.getZ()-0.1);
		if(keys[SDLK_i])
			loc.setZ(loc.getZ()+0.1);
		if(keys[SDLK_u])
			loc.setY(loc.getY()-0.1);
		if(keys[SDLK_o])
			loc.setY(loc.getY()+0.1);
		std::cout << loc.getX() << " " << loc.getY() << " " << loc.getZ() << std::endl;
}
