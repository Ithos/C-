#ifndef KNIFE_H
#define KNIFE_H
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <GL/glu.h>
#include <iostream>
#include <string>
#include <vector>
#include "enemy.h"
#include "vector3d.h"
#include "collision.h"

class knife{
	std::vector<unsigned int> frames;
	bool inUse;//Indicamos si se esta usando el cuchillo
	bool testing;//Esta variable indica si estamos probando la posición del cuchillo
	int damage;
	int delay;//Retardo para usar el cuchillo
	int wepDelay;//Tiempo que el arma esta deshabilitada
	int lastUse;//Tiempo que hace que se empleo el cuchillo por ultima vez
	vector3d loc;//Posición y rotación del cuchillo
	int curframe;
	public:
	knife(std::vector<unsigned int>& f,int dmg,int del,int wdel,const vector3d& location);
	int setKnife(std::vector<enemy*>& enemies,const vector3d& dir,const vector3d& loc);
	bool update();
	void show();
	bool isUsing();
	int getDamage();
	void test();//Esta función es igual que para las armas, solo sirve para colocarlo en posición
};

#endif
