#ifndef WEAPON_H
#define WEAPON_H

#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <cstdlib>
#include "vector3d.h"
#include "particlesystem.h"

struct attachment{
	//Nota: no tengo ni idea de como o para que se usa esto, lo he copiado y dejado aqui para un posible uso en el futuro
	int id;
	unsigned int mesh;
	vector3d position;
	vector3d rotation;
	vector3d scale;
	attachment(int i,unsigned int m,const vector3d& p,const vector3d& r,const vector3d& s) : id(i),mesh(m),position(p),rotation(r),scale(s) {}
};

class weapon{
	std::vector<unsigned int> frames;
	std::vector<attachment*> attachments;
	unsigned int outerview;
	unsigned int normalanimation;
	unsigned int fireanimation;
	unsigned int reloadanimation;
	vector3d expected_pos;
	vector3d expected_rot;
	vector3d position;//Posicion y rotación cuando no disparamos
	vector3d rotation;
	vector3d aimposition;//Posición y rotación cuando apuntamos
	vector3d aimrotation;
	vector3d curpos;//Posición y rotación actual
	vector3d currot;
	
	vector3d particlePosition;
	
	float precision,aimprecision;//Diferentes precisiones de las armas
	int damage;//daño causado por el arma
	bool aiming,reloading,automatic,fired,isTest;
	
	unsigned int maxBullets;
	unsigned int bullets;
	unsigned int allBullets;
	unsigned int lastshot;//Almacena el numero de frames que hace que se disparó el arma por última vez
	
	unsigned int speed;//velocidad del arma
	float changeSpeed;
	
	unsigned int curframe;
	unsigned int curmode;//1-normal 2-disparando 3-recargando
	
	std::string name;
	
	particlesystem* particles;
	
	public:
	
	weapon(std::vector<unsigned int>& anim,unsigned int ol,unsigned int na,unsigned int fa,unsigned int ra,vector3d pos,vector3d rot,vector3d apos,vector3d arot,float prec,float aprec,int dmg,int maxb,int allbul,int sp,const char* nam,bool aut,const vector3d& partPos,particlesystem* part,float cs=0.3);
	~weapon();
	
	void update();
	void show();

	bool fire(vector3d& direction,vector3d camdirection);//Retornamos el resultado (true si el disparo se realizo, false si el disparo no se realizo)
	void stopfire();
	void reload();
	void aim();
	void test(attachment* att=NULL);//Esta función nos permitirá probar la colocación de las armas
	void addBullets(unsigned int num);
	void setBullets(unsigned int num);
	
	int getDamage();
	int getAmmo();
	int getTotalAmmo();
	std::string getName();
	std::vector<unsigned int>& getAnimation();
	bool isAimed();
	unsigned int getOuterView();
	
	void addAttachment(int i,unsigned int m,const vector3d& p,const vector3d& r,const vector3d& s,const vector3d& nnp,const vector3d& nap,const vector3d& nnr,const vector3d& nar);
	void showParticleSystem(camera& cam);
	
};

#endif
