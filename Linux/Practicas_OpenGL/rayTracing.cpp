#include "rayTracing.h"

bool raysphere(float xc,float yc,float zc,float xd,float yd,float zd,float xs,float ys,float zs,float r, float* dist, coordinate* point){
	/*Para sacar estas ecuaciones simplemente tomamos la ecuacion parametrica de una recta dado un punto de inicio y un vector direccion
	y la ecuacion de una esfera dado un centro y un radio, planteamos las ecuaciones de la interseccion de ambas y despejamos el parametro
	de la recta, si existe, hay colision*/
	float b=2*(xd*(xc-xs)+yd*(yc-ys)+zd*(zc-zs));//Estos parametros salen de una ecuacion de segundo grado a*x^2+b*x+c=0, teniendo en cuenta que al normalizar a = 1
	float c=xs*xs-2*xs*xc+xc*xc+ys*ys-2*ys*yc+yc*yc+zs*zs-2*zs*zc+zc*zc-r*r;
	float disc=b*b-4*c;//Esto es el discriminante que sale al intentar resolver la ecuación de 2º grado
	
	if(dist!=NULL){//Si nos han pasado un puntero para almacenar la distancia se la damos
		(*dist)=(-b+sqrt(disc))/2;
		if(point!=NULL){ //Si nos han pasado un puntero para almacenar el punto de colisión se lo damos
			point->x=xs+(*dist)*xd;
			point->y=ys+(*dist)*yd; 
			point->z=zs+(*dist)*zd; 
		} 
	}
	
	if(disc<0)return false;//Si el discriminante es < 0 la ecuacion no se puede resolver y por lo tanto no hay colision
	else return true;
	
}

bool rayplane(float xn,float yn,float zn,float xs,float ys,float zs,float xd,float yd,float zd,coordinate p1,coordinate p2,coordinate p3,coordinate p4, float* dist, coordinate* point){
	/*
	 * En este caso calcularemos el punto de colisión entre un plano y una recta, para ello emplearemos la ecuación parametrica de la recta
	 * y la ecuación del plano de punto y normal, despues sustituiremos en la ecuación del plano las coordenadas x,y,z por las de la recta
	 * y despejaremos el parámetro, teniendo en cuenta que si no se puede calcular recta y plano son paralelos, despues comprobaremos si le punto 
	 * de colisión esta dentro del paralelogramo en el que estamos interesados, para ello dividimos el paralelogramo en dos triangulos y trazamos
	 * rectas desde el punto de colisión hasta los vértices de tal forma que cada uno de los dos triangulos queda dividido en tres, después comparamos 
	 * el area de estos triangulos con los del triángulo al que dividen, si la suma de su área es igual a la de este triangulo, entonces el punto
	 * de colisión se encuentra dentro de ese triángulo.
	*/
	
	float a=xd*xn+yd*yn+zd*zn;
	if(a==0)return false;//Si esto es 0 recta y plano son paralelos
	float t=((p1.x*xn+p1.y*yn+p1.z*zn-xn*xs-yn*ys-zn*zs)/a);//Calculamos el parámetro de la ecuación paramétrica de la recta
	if(t<0)return false;//Si el parámetro es negativo la colision ocurre mas atras del punto de inicio de la recta y en dirección contraria al vector de esta, por lo que no colisionan
	float x=xs+t*xd;//Calculamos el punto de colisión
	float y=ys+t*yd;
	float z=zs+t*zd;
	coordinate cp(x,y,z);
	//Ahora comprobamos que las areas de los triangulos son iguales mediante una diferencia para evitar que la precision del calculo cause problemas
	if(abs(trianglearea(p1,p3,p4)-trianglearea(p1,p4,cp)-trianglearea(p1,p3,cp)-trianglearea(p3,p4,cp))<0.000000001 || abs(trianglearea(p1,p2,p3)-trianglearea(p1,p2,cp)-trianglearea(p1,p3,cp)-trianglearea(p2,p3,cp))<0.000000001){
		 
		 if(dist!=NULL){//Si nos han pasado un puntero para almacenar la distancia se la damos
			(*dist)=t;
			if(point!=NULL){//Si nos han pasado un puntero para almacenar el punto de colisión se lo damos
				point->x=x;
				point->y=y;
				point->z=z;
			}
		 }
		 
		 return true;
	 }
	return false;
}
bool sphereplane(coordinate &sp, coordinate vn,coordinate p1,coordinate p2,coordinate p3,coordinate p4,float r){
	/*
	 * 1->Trazamos una línea desde el centro de la esfera hasta el plano tomando como vector director la normal al plano
	 * 2->Comprobamos la distancia entre la esfera y el plano, si la distancia es menor que el rádio de la esfera entonces tenemos una colisión
	 * 3->Si hay una colisión recolocaremos la esfera moviéndola segun la normal al plano 
	 */
	 float dist1=0,dist2=0;//Cada uno almacenará la distancia de la esfera al plano en una direccion, según el vector normal al plano o en sentido contrario a este
	 if(rayplane(-vn.x,-vn.y,-vn.z,sp.x,sp.y,sp.z,vn.x,vn.y,vn.z,p1,p2,p3,p4,&dist1)||rayplane(vn.x,vn.y,vn.z,sp.x,sp.y,sp.z,-vn.x,-vn.y,-vn.z,p1,p2,p3,p4,&dist2)){
		 //Empleamos la función rayplane dos veces, una por cada lado del plano donde puede estar la esfera, una de estas funciones nos dará la distancia entre esfera y plano
		 if(dist1>r||dist2>r)//Si la distancia es mayor que el radio de la esfera devolvemos false
			 return false;
		 if(dist1>0){
			 sp.x=sp.x-vn.x*(r-dist1);//Segun cual sea el lado por el que colisiona movemos la esfera la distancia necesaria según la normal al plano
			 sp.y=sp.y-vn.y*(r-dist1);
			 sp.z=sp.z-vn.z*(r-dist1);
		 }else{
			 sp.x=sp.x+vn.x*(r-dist2);
			 sp.y=sp.y+vn.y*(r-dist2);
			 sp.z=sp.z+vn.z*(r-dist2);
		 }
		 return true;
	 }
	 return false;
}

bool spheresphere(coordinate &c1,float r1,coordinate c2,float r2){
	float dist=pointdistance(c1,c2);
	if(dist<=(r1+r2)*(r1+r2)){
		float a=sqrt(dist)-(r1+r2);
		coordinate vec(c2.x-c1.x,c2.y-c1.y,c2.z-c1.z);
		float len=sqrt((vec.x*vec.x+vec.y*vec.y+vec.z*vec.z));
		vec.x/=len;
		vec.y/=len;
		vec.z/=len;
		c1.x=c1.x+vec.x*a; 
		c1.y=c1.y+vec.y*a;
		c1.z=c1.z+vec.z*a;
		
		return true;
	}
	return false;
}

float trianglearea(coordinate p1,coordinate p2,coordinate p3){
	
	float a=sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y)+(p2.z-p1.z)*(p2.z-p1.z));//Calculamos la longitud de los lados del triangulo
	float b=sqrt((p3.x-p2.x)*(p3.x-p2.x)+(p3.y-p2.y)*(p3.y-p2.y)+(p3.z-p2.z)*(p3.z-p2.z));
	float c=sqrt((p3.x-p1.x)*(p3.x-p1.x)+(p3.y-p1.y)*(p3.y-p1.y)+(p3.z-p1.z)*(p3.z-p1.z));
	
	float s=(a+b+c)/2;//Ahora usamos la formula de heron para calcular el area del triangulo
	
	return (sqrt(s*(s-a)*(s-b)*(s-c)));
}

float pointdistance(coordinate p1,coordinate p2){
	coordinate vec(p2.x-p1.x,p2.y-p1.y,p2.z-p1.z);
	return /*sqrt*/ (vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}
