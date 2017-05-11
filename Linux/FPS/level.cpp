#include "level.h"

level::level(const char* c,unsigned int map,std::vector<collisionPlane>& coll,std::vector<vector3d>& sp,unsigned int* sk){
	mesh=map;
	cp=coll;
	spawnPoints=sp;
	
	for(int i=0;i<cp.size();i++){
		if(cp[i].getId()==200){
			spawnPoints.push_back(cp[i].getPoints()[0]);
			cp.erase(cp.begin()+i);
		}
	}
	
	name=c;
	sky=new skybox(sk[0],sk[1],sk[2],sk[3],sk[4],sk[5],500.0);
}

level::~level(){
	delete sky;
}

void level::update(){
	
}

void level::show(){
	glCallList(mesh);
}

std::vector<collisionPlane>& level::getCollisionPlane(){
	return cp;
}

std::string& level::getName(){
	return name;
}

void level::change(const char* c,unsigned int map,std::vector<collisionPlane>& coll,std::vector<vector3d>& sp){
	mesh=map;
	cp=coll;
	spawnPoints=sp;
	name=c;
}

std::vector<vector3d>& level::getSpawnPoints(){
	return spawnPoints;
}

skybox* level::getSkybox(){
	return sky;
}

vector3d level::getRandomSpawnPoint(){
	int i=rand()%spawnPoints.size();
	return spawnPoints[i];
}
