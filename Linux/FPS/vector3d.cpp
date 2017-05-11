#include "vector3d.h"

vector3d::vector3d(){
	x=y=z=0;
}

vector3d::vector3d(float a,float b){
	x=a;
	y=b;
	z=0;
}

vector3d::vector3d(float a,float b,float c){
	x=a;
	y=b;
	z=c;
}
		
float vector3d::dotProduct(const vector3d& vec)const{//Producto escalar
	return (x*vec.getX()+y*vec.getY()+z*vec.getZ());
}

vector3d vector3d::crossProduct(const vector3d& vec) const {//Producto vectorial
	return vector3d(y*vec.getZ()-z*vec.getY(),x*vec.getZ()-z*vec.getX(),x*vec.getY()-y*vec.getX());
}

float vector3d::lenght()const{
		return sqrt(x*x+y*y+z*z);
}

void vector3d::normalize(){
	float len=lenght();
	if(len!=0){//Evitamos dividir entre 0
		x/=len;
		y/=len;
		z/=len;
	}
}
		
void vector3d::change(float a,float b,float c){
	x=a;
	y=b;
	z=c;
}

void vector3d::change(const vector3d& vec){
	x=vec.getX();
	y=vec.getY();
	z=vec.getZ();
}
		
void vector3d::setX(float a){
	x=a;
}

void vector3d::setY(float a){
	y=a;
}

void vector3d::setZ(float a){
	z=a;
}
		
vector3d vector3d::operator+(const vector3d& vec)const{
		return vector3d(x+vec.getX(),y+vec.getY(),z+vec.getZ());
}

vector3d vector3d::operator-(const vector3d& vec)const{
		return vector3d(x-vec.getX(),y-vec.getY(),z-vec.getZ());
}

vector3d vector3d::operator*(float a)const{
		return vector3d(x*a,y*a,z*a);
}

vector3d vector3d::operator/(float a)const{
	if(a!=0){
		return vector3d(x/a,y/a,z/a);
	}else{
		return vector3d();
	}
}
		
vector3d& vector3d::operator+=(const vector3d& vec){
	x+=vec.getX();
	y+=vec.getY();
	z+=vec.getZ();
	return *this;
}

vector3d& vector3d::operator-=(const vector3d& vec){
	x-=vec.getX();
	y-=vec.getY();
	z-=vec.getZ();
	return *this;
}
vector3d& vector3d::operator*=(float a){
	x*=a;
	y*=a;
	z*=a;
	return *this;
}

vector3d& vector3d::operator/=(float a){
	if(a!=0){
		x/=a;
		y/=a;
		z/=a;
	}
	return *this;
}
		
bool vector3d::operator==(const vector3d& vec)const{
	return (x==vec.getX()&&y==vec.getY()&&z==vec.getZ());
}

bool vector3d::operator!=(const vector3d& vec)const{
	return (x!=vec.getX()&&y!=vec.getY()&&z!=vec.getZ());
}

std::ostream& operator<<(std::ostream& out, const vector3d& vec){
	out << "x=" << vec.getX() << " " <<  "y=" << vec.getY()  << " " <<  "z=" << vec.getZ() << std::endl;
	return out; 
}
