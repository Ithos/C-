#ifndef SCENELOADER_H
#define SCENELOADER_H
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL/SDL_image.h>
#include "mesh.h"

class meshLoader{
	std::vector<meshClass*> meshes;
	void recursiveProcess(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh,const aiScene* scene);
	unsigned int loadTexture(const char* filename, bool generate=false);
	public:
	meshLoader(const char* filename);
	~meshLoader();
	void draw(unsigned int programId);//Se podrían emplear distintos shaders para distintos objetos, pero en este programa no lo vamos a hacer
	std::vector<meshClass*>& getMeshes();
};

#endif
