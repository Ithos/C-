#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>//Contiene funciones para trabajar con rangos de elementos
#include <fstream>
#include <iostream>
#include <cstdio>

#ifndef OBJLOADER_H
#define OBJLOADER_H

struct coordinate{//Aqui almacenamos las coordenadas de los vertices
	float x,y,z;
	coordinate(float a,float b,float c);//Constructor
};

struct face{//Aqui almacenamos diversos parametros de las caras
	int facenum;//Almacena el numero de la cara
	bool four;//Indica si la cara esta formada por cuadrados o por triangulos
	int faces[4];//Almacena las coordenadas de los vertices de la cara
	int texcoor[4];//Coordenadas de la textura
	int mat;//Identificador del material
	face(int facen,int f1,int f2,int f3,int t1,int t2,int t3,int m);//Sobrecargamos el constructor de tal forma que si la cara esta formada por triangulos sera este constructor el que salte
	face(int facen,int f1,int f2,int f3,int f4,int t1,int t2,int t3,int t4,int m);
};

struct material{//Aqui almacenaremos el material que vendra dado en el archivo .mtl
	std::string name;
	float alpha,ns,ni;
	float dif[3],amb[3],spec[3];
	float illum;
	int texture;
	material(const char* na,float al,float nS,float nI,float *d,float *a,float *s,float i,int tex);//Constructor, faltan parametros
};

struct texcoord{//Para almacenar texturas uv
	float u,v;
	texcoord(float a,float b);
};

class objLoader{
	std::vector<std::string*> coord;//Este vector lo lee todo como chars, por lo que practicamente es capaz de leerlo todo
	std::vector<coordinate*> vertex;//Aqui almacenamos los vertices
	std::vector<face*> faces;//En este vector almacenamos las caras
	std::vector<coordinate*> normals;//Aqui almacenamos los vectores normales a las caras
	std::vector<material*> materials;//Aqui almacnamos los materiales
	std::vector<texcoord*> texturecoord;
	std::vector<unsigned int>texture;
	std::vector<unsigned int>lists;
	std::vector<coordinate*>vertexnormals;//Aqui almacenamos las normales calculadas en los vertices ponderando las normales en las caras
	bool ismaterial,isnormals,istexture,smoothnormals;
	unsigned int loadTexture(const char* filename, bool generate=true);
	void clear();
	void smooth();//Esta funcion se encarga de hacer una media de las normales de las caras adyacentes a cada vertice
	
	public:
	
	objLoader();//Constructor de la clase
	~objLoader();//Destructor, donde borraremos todo
	int loadObject(std::string filename);//Funcion donde cargaremos el objeto
	
};


#endif
