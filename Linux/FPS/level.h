#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "skybox.h"
#include "collisionPlane.h"
#include "vector3d.h"

class level{
	unsigned int mesh;
	std::vector<collisionPlane> cp;
	std::vector<vector3d> spawnPoints;
	std::string name;
	skybox* sky;
	public:
	level(const char* c,unsigned int map,std::vector<collisionPlane>& coll,std::vector<vector3d>& sp,unsigned int* sk);
	~level();
	void update();
	void show();
	std::vector<collisionPlane>& getCollisionPlane();
	std::string& getName();
	void change(const char* c,unsigned int map,std::vector<collisionPlane>& coll,std::vector<vector3d>& sp);
	std::vector<vector3d>& getSpawnPoints();
	skybox* getSkybox();
	vector3d getRandomSpawnPoint();
};


#endif
