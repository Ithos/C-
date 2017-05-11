#include "matrix.h"

fvec::fvec(){//Esto no hace na

}

fvec::fvec(float a){//Damos los primeros valores del vector
	f[0]=a;
}

fvec::fvec(float a,float b){
	f[0]=a;
	f[1]=b;
}

fvec::fvec(float a,float b,float c){
	f[0]=a;
	f[1]=b;
	f[2]=c;
}

fvec::fvec(float a,float b,float c,float d){
	f[0]=a;
	f[1]=b;
	f[2]=c;
	f[3]=d;
}

fvec::fvec(float* tmp,int d){//Pasamos los d primeros valores del vector como un vector de float
	for(int i=0;i<d;i++)f[i]=tmp[i];
}

float& fvec::operator[](int i){//Sobreescribimos el operador[] para devolver un elemento del vector
	return f[i];
}

matrix::matrix(){//Por defecto construimos una matriz identidad 4x4
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			m[i][j]=(i==j);
		}
	}
	dimension=4;
	error=0;
}

matrix::matrix(int n){//Si indicamos la dimensión construimos una matriz identidad nxn
	if(n<=0){
		error=1;
		dimension=1;
	}else{
		error=0;
		dimension=n;
	}
	dimension=n;
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			m[i][j]=(i==j);
		}
	}	
}

matrix::matrix(float* f,int n){//Podemos pasar un vector de floats para dar los valores de la matriz en el constructor
	if(n<=0){
		error=1;
		dimension=1;
	}else{
		error=0;
		dimension=n;
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			m[i][j]=f[i*n+j];
		}
	}
}

void matrix::setDimension(int n){//Cambiamos el valor de la dimensión de la matriz (solo creamos matrices cuadradas)
	dimension=n;
}

void matrix::change(float* f,int n){//Cambiamos los valores de la matriz
	if(n<=0){
		error=1;
		dimension=1;
	}else{
		error=0;
		dimension=n;
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			m[i][j]=f[i*n+j];
		}
	}
}

void matrix::transpose(){//Transponemos la matriz
	float tmp;
	for(int i=0;i<dimension;i++){
		for(int j=i;j<dimension;j++){
			tmp=m[i][j];
			m[i][j]=m[j][i];
			m[j][i]=tmp;
		}
	}
}

void matrix::inverse(){//Calculamos la inversa de la matriz mediante el método general
	float det=determinant();
	if(det==0){
		error=2;
		return;
	}
	matrix tmp(dimension-1);
	matrix result(dimension);
	int indexx=0,indexy=0;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			indexy=indexx=0;
			for(int k=0;k<dimension;k++){
				if(k==i)continue;
				indexx=0;
				for(int l=0;l<dimension;l++){
					if(l==j)continue;
					//std::cout << indexy <<  ' ' << indexx << std::endl;
					tmp[indexy][indexx]=m[k][l];
					indexx++;
				}
				indexy++;
			}
			tmp.printDebug();
			result[i][j]=tmp.determinant();
			if((!(i&1) && (j&1)) || ((i&1) && !(j&1)))result[i][j]*=-1;
		}
	}
	result.printDebug();
	result.transpose();
	result=result/det;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			m[i][j]=result[i][j];
		}
	}
}

void matrix::loadIdentity(){//Cargamos la identidad del orden adecuado 
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			m[i][j]=(i==j);
		}
	}
}

float matrix::determinant(){//Calculamos el determinante de la matriz
	if(dimension==1)return m[0][0];//Si la dimensión es 1 ó 2 empleamos una fórmula simple
	if(dimension==2)return (m[0][0]*m[1][1]-m[0][1]*m[1][0]);
	matrix mats[dimension];//Si no empleamos la fórmula genérica
	int indexx=0;
	int indexy=0;
	float res=0;
	for(int i=0;i<dimension;i++){//Reducimos las dimensiones de la matriz, multiplicamos por el elemento asociado y sumamos o restamos
		mats[i].setDimension(dimension-1);
		indexx=indexy=0;
		for(int j=1;j<dimension;j++){
			indexx=0;
			for(int k=0;k<dimension;k++){
				if(k==i)continue;
				mats[i][indexy][indexx]=m[j][k];
				indexx++;
			}
			indexy++;
		}
		if(i&1)res-=m[0][i]*mats[i].determinant();
		else res+=m[0][i]*mats[i].determinant();		
	}
	return res;
}

fvec matrix::getColoumn(int n){
	fvec vec;
	for(int i=0;i<dimension;i++)vec[i]=m[i][n];
	return vec;
}

void matrix::printDebug(){
	std::cout << "Error code: " << error << std::endl;
	error=0;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			std::cout << m[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

fvec& matrix::operator[](int n){
	return m[n];
}

matrix matrix::operator*(float f){//Multiplicamos cada elemento por el float que recibimos
	matrix result;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			result[i][j]=m[i][j]*f;
		}
	}
	return result;
}

matrix matrix::operator*(matrix& m2){
	if(dimension!=m2.dimension){
		error=3;
		return matrix();
	}
	matrix result(dimension);
	
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			result[i][j]=0;
		}
	}
	
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			for(int k=0;k<dimension;k++){
				result[i][j]+=m[i][k]*m2[k][j];
			}
		}
	}
	
	return result;
}

vector3d matrix::operator*(vector3d m2){
	if(dimension!=3){
		error=4;
		return vector3d(0,0,0);
	}
	vector3d result;
	result.setX(m2.dotProduct(vector3d(m[0].f[0],m[0].f[1],m[0].f[2])));
	result.setY(m2.dotProduct(vector3d(m[1].f[0],m[1].f[1],m[1].f[2])));
	result.setZ(m2.dotProduct(vector3d(m[2].f[0],m[2].f[1],m[2].f[2])));
	return result;
}

matrix matrix::operator+(matrix& m2){
	if(dimension!=m2.dimension){
		error=4;
		return matrix();
	}
	matrix result;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			result[i][j]=m[i][j]+m2[i][j];
		}
	}
	return result;
}

matrix matrix::operator-(matrix& m2){
	if(dimension!=m2.dimension){
		error=4;
		return matrix();
	}
	matrix result;
	for(int i=0;i<dimension;i++)
		for(int j=0;j<dimension;j++)
			result[i][j]=m[i][j]-m2[i][j];
	return result;
}

matrix matrix::operator/(float f){
	if(f==0){
		error=4;
		return matrix(dimension);
	}
	matrix result;
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			result[i][j]=m[i][j]/f;
		}
	}
	return result;
}
