#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <iostream>
#include <GL/gl.h>
#include <vector>
#include <SDL/SDL.h>
#include "vector3d.h"
#include "cameraClass.h"

class particlesystem{
	struct particle{//Struct que contendrá las carácteristicas de las partículas
		vector3d position;
		vector3d rotation;
		vector3d speed;
		vector3d startcolor;
		vector3d endcolor;
		float size;
		Uint32 lastUpdate;
		vector3d color;
		int lifetime;
		int totalLifetime;
		bool dead;
		particle();
		particle(const vector3d& pos,const vector3d& rot,const vector3d& col,const vector3d& sp,float s);
		bool update();
		void show(camera& cam);
	};
	std::vector<particle> particles;
	int particleNum;//Número de particula
	vector3d position;
	bool useTexture;
	unsigned int texture;
	Uint32 active;
	bool isActive;
	vector3d startColor,endColor,force;
	float minSize,maxSize,minSpeed,maxSpeed;
	int minLifetime,maxLifetime;
	int numparticles;//Total de partículas
	public:
	particlesystem(int db,const vector3d& pos,float ms,float mas,float minSp,float maxSp,const vector3d& f,const vector3d& startcolor,const vector3d& endcolor,int minlt,int maxlt,int nm,unsigned int tex=-1);
	~particlesystem();
	void generateParticle(particle& p);
	void setPosition(const vector3d& pos);
	void update();
	void show(camera& cam);
	bool isParticleSystemActive();
	void setNumParticles(int n);
	void setForce(const vector3d& f);
};

#endif
