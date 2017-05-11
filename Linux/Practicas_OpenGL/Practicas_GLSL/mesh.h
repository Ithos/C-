#ifndef MESH_H
#define MESH_H
#include <iostream>
#include "GLee.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>
#include "meshData.h"

class meshClass{
	std::vector<vertexData> data; //Aqui almacenamos datos de los vertices
	std::vector<textureData> textures; //aqui almacenamos datos de las texturas
	std::vector<unsigned int> indexes;//Almacenamos indices
	unsigned int VBO;//Vertex buffer object
	unsigned int IND;//Inex buffer
	public:
	meshClass(std::vector<vertexData>* vd,std::vector<unsigned int>* id,std::vector<textureData>* td=NULL);
	~meshClass();
	void draw(unsigned int programId);
};
#endif
