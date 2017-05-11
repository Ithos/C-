#include <iostream>
#include <vector>
#include "vector3d.h"

class fvec{//Vector de floats
	public:
		float f[4];
		fvec();
		fvec(float a);
		fvec(float a,float b);
		fvec(float a,float b,float c);
		fvec(float a,float b,float c,float d);
		fvec(float* tmp,int d);
		float& operator[](int i);
};

class matrix{//Clase para operar con matrices
	fvec m[4];
	int dimension;
	int error;//Nos indica si se ha producido un error durante la operación
	public:
		matrix();
		matrix(int n);
		matrix(float* f,int n);
		void setDimension(int n);
		void change(float* f,int n);
		void transpose();
		void inverse();
		void loadIdentity();
		float determinant();
		fvec getColoumn(int n);
		void printDebug();
		fvec& operator[](int n);
		matrix operator*(float f);
		matrix operator*(matrix& m2);
		vector3d operator*(vector3d m2);
		matrix operator+(matrix& m2);
		matrix operator-(matrix& m2);
		matrix operator/(float f);	
};
