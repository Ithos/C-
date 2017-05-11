#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include "collision.h"
#include "collisionPlane.h"
#include "collisionSphere.h"
#include "vector3d.h"
#include "cameraClass.h"
#include "weapon.h"
#include "knife.h"
#include "grenade.h"

class player{
	std::string name;
	collisionSphere cs;//Habrá que cambiar el sistema de colisiones de una unica esféra a multiples esféras
	int health;
	vector3d gravity;
	vector3d direction;
	bool ground,collision,sprint;
	float sprintSpeed,normalSpeed;
	float mousespeed;
	float stamina;
	int points;
	std::vector<weapon*> weapons;
	std::vector<grenade*> grenades;
	knife* k;
	int curweapon;
	bool isWeapon;
	
	int camBounce;
	bool headUp;
	
	int id;//Identificador de cliente para el servidor
	bool ready;//Indica al servidor si el cliente esta listo
	
	public:
	camera cam;//No me gusta dejar la camara como public, probablemente lo cambie a private
	player(const char* n,collisionSphere ccs,float ssped,float nspeed,float mspeed,weapon* wep,knife* kn,grenade* g);
	~player();
	void update(std::vector<collisionPlane>& collplane,const vector3d& newPos,std::vector<enemy*>& enemies);
	void show();
	void setPosition(vector3d pos);
	
	std::string getName();
	int getHealth();
	collisionSphere getCollisionSphere();
	int getPoints();
	
	void setHealth(int n);
	void addPoints(int n);
	
	void addWeapon(weapon* wep);
	void changeWeapon(int n);
	void changeWeaponDown();
	void changeWeaponUp();
	weapon* getEquippedWeapon();
	void haveWeapon(bool b);
	
	void setKnife(std::vector<enemy*>& enemies);
	void dropGrenade();
	std::vector<grenade*>* getGrenades();
	int curgrenade;//Indice de la granada seleccionada
	
	void setJump();
	void setSprint();
	void stopSprint();
	
	void setId(int i);
	int getId();
	
	bool getReady();
	int getCurframe();
	vector3d getRotation();
	int getWeaponIndex();
	void showKnife();
};

#endif
