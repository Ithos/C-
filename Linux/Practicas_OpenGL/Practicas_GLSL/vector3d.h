#include <cmath>
#include <iostream>

#ifndef VECTOR3D_H
#define VECTOR3D_H

class vector3d{
	float x,y,z;
	public:
		vector3d();
		vector3d(float a,float b);
		vector3d(float a,float b,float c);
		
		float dotProduct(const vector3d& vec);
		vector3d crossProduct(const vector3d& vec);
		float lenght();
		void normalize();
		
		void change(float a,float b,float c);
		void change(const vector3d& vec);
		
		void setX(float a);
		float getX() const {return x;};
		void setY(float a);
		float getY() const {return y;};
		void setZ(float a);
		float getZ() const {return z;};
		
		//Sobrecargamos las funciones de varios operadores para operar con vectores
		
		vector3d operator+(const vector3d& vec);
		vector3d operator-(const vector3d& vec);
		vector3d operator*(float a);
		vector3d operator/(float a);
		
		vector3d& operator+=(const vector3d& vec);
		vector3d& operator-=(const vector3d& vec);
		vector3d& operator*=(float a);
		vector3d& operator/=(float a);
		
		bool operator==(const vector3d& vec);
		bool operator!=(const vector3d& vec);
		
		//Para poder sobrescribir el operador salida tenemos que darle a la función el mismo acceso que del que tiene iostream, para eso le damos la categoría friend
		friend std::ostream& operator<<(std::ostream& out, const vector3d& vec);
};

#endif
