#include "collision.h"

bool collision::raysphere(float xc,float yc,float zc,float xd,float yd,float zd,float xs,float ys,float zs,float r, float* dist, vector3d* point){
	/*Para sacar estas ecuaciones simplemente tomamos la ecuacion parametrica de una recta dado un punto de inicio y un vector direccion
	y la ecuacion de una esfera dado un centro y un radio, planteamos las ecuaciones de la interseccion de ambas y despejamos el parametro
	de la recta, si existe, hay colision*/
	float b=2*(xd*(xc-xs)+yd*(yc-ys)+zd*(zc-zs));//Estos parametros salen de una ecuacion de segundo grado a*x^2+b*x+c=0, teniendo en cuenta que al normalizar a = 1
	float c=xs*xs-2*xs*xc+xc*xc+ys*ys-2*ys*yc+yc*yc+zs*zs-2*zs*zc+zc*zc-r*r;
	float disc=b*b-4*c;//Esto es el discriminante que sale al intentar resolver la ecuación de 2º grado
	
	if(dist!=NULL){//Si nos han pasado un puntero para almacenar la distancia se la damos
		(*dist)=(-b+sqrt(disc))/2;
		if(point!=NULL){ //Si nos han pasado un puntero para almacenar el punto de colisión se lo damos
			point->setX(xs+(*dist)*xd);
			point->setY(ys+(*dist)*yd); 
			point->setZ(zs+(*dist)*zd); 
		} 
	}
	
	if(disc<0)return false;//Si el discriminante es < 0 la ecuacion no se puede resolver y por lo tanto no hay colision
	else return true;
	
}

bool collision::rayplane(float xn,float yn,float zn,float xs,float ys,float zs,float xd,float yd,float zd,const vector3d& p1,const vector3d& p2,const vector3d& p3,const vector3d& p4, float* dist, vector3d* point){
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
	float t=((p1.getX()*xn+p1.getY()*yn+p1.getZ()*zn-xn*xs-yn*ys-zn*zs)/a);//Calculamos el parámetro de la ecuación paramétrica de la recta
	if(t<0)return false;//Si el parámetro es negativo la colision ocurre mas atras del punto de inicio de la recta y en dirección contraria al vector de esta, por lo que no colisionan
	float x=xs+t*xd;//Calculamos el punto de colisión
	float y=ys+t*yd;
	float z=zs+t*zd;
	vector3d cp(x,y,z);
	//Ahora comprobamos que las areas de los triangulos son iguales mediante una diferencia para evitar que la precision del calculo cause problemas
	if(abs(trianglearea(p1,p3,p4)-trianglearea(p1,p4,cp)-trianglearea(p1,p3,cp)-trianglearea(p3,p4,cp))<0.000000001 || abs(trianglearea(p1,p2,p3)-trianglearea(p1,p2,cp)-trianglearea(p1,p3,cp)-trianglearea(p2,p3,cp))<0.000000001){
		 
		 if(dist!=NULL){//Si nos han pasado un puntero para almacenar la distancia se la damos
			(*dist)=t;
			if(point!=NULL){//Si nos han pasado un puntero para almacenar el punto de colisión se lo damos
				point->setX(x);
				point->setY(y);
				point->setZ(z);
			}
		 }
		 
		 return true;
	 }
	return false;
}

float collision::trianglearea(const vector3d& p1,const vector3d& p2,const vector3d& p3){
	
	//Formula vectorial
	/*vector3d v1 = p2 - p1;
	vector3d v2 = p3 - p1;
	
	vector3d res = v1.crossProduct(v2);
	
	return abs(res.lenght()/2);*/
	
	float a=sqrt((p2.getX()-p1.getX())*(p2.getX()-p1.getX())+(p2.getY()-p1.getY())*(p2.getY()-p1.getY())+(p2.getZ()-p1.getZ())*(p2.getZ()-p1.getZ()));//Calculamos la longitud de los lados del triangulo
	float b=sqrt((p3.getX()-p2.getX())*(p3.getX()-p2.getX())+(p3.getY()-p2.getY())*(p3.getY()-p2.getY())+(p3.getZ()-p2.getZ())*(p3.getZ()-p2.getZ()));
	float c=sqrt((p3.getX()-p1.getX())*(p3.getX()-p1.getX())+(p3.getY()-p1.getY())*(p3.getY()-p1.getY())+(p3.getZ()-p1.getZ())*(p3.getZ()-p1.getZ()));
	
	float s=(a+b+c)/2;//Ahora usamos la formula de heron para calcular el area del triangulo
	
	return (sqrt(s*(s-a)*(s-b)*(s-c)));
}

bool collision::sphereplane(vector3d &sp, const vector3d& vn,const vector3d& p1,const vector3d& p2,const vector3d& p3,const vector3d& p4,float r,bool up,bool* ramp){
	/*
	 * 1->Trazamos una línea desde el centro de la esfera hasta el plano tomando como vector director la normal al plano
	 * 2->Comprobamos la distancia entre la esfera y el plano, si la distancia es menor que el rádio de la esfera entonces tenemos una colisión
	 * 3->Si hay una colisión recolocaremos la esfera moviéndola segun la normal al plano 
	 */
	 float dist1=0,dist2=0;//Cada uno almacenará la distancia de la esfera al plano en una direccion, según el vector normal al plano o en sentido contrario a este
	 if(rayplane(-vn.getX(),-vn.getY(),-vn.getZ(),sp.getX(),sp.getY(),sp.getZ(),vn.getX(),vn.getY(),vn.getZ(),p1,p2,p3,p4,&dist1)||rayplane(vn.getX(),vn.getY(),vn.getZ(),sp.getX(),sp.getY(),sp.getZ(),-vn.getX(),-vn.getY(),-vn.getZ(),p1,p2,p3,p4,&dist2)){
		 //Empleamos la función rayplane dos veces, una por cada lado del plano donde puede estar la esfera, una de estas funciones nos dará la distancia entre esfera y plano
		 if(dist1>r||dist2>r)//Si la distancia es mayor que el radio de la esfera devolvemos false
			 return false;
		 vector3d tmp=p3;
		 vector3d vup=tmp-p2;//Para evitar fallos al calcular colisiones contra planos verticales realizamos una comprobación adiccional
		 vup.normalize();
		 bool vertical=vector3d(0,1,0).dotProduct(vup)>0.99;
		 if(up&&!vertical){//Esta variable nos indica si la colisión debe permitir ascender por planos inclinados hasta un cierto límite
			 //Calculamos la inclinación del plano y adaptamos el vector mediante el cual desplazamos la esfera en consecuencia
			 tmp=p4;
			 vector3d diagonal1=tmp-p2;
			 tmp=p3;
			 vector3d diagonal2=tmp-p2;
			 vector3d reference1=diagonal1;
			 reference1.setY(0.0);
			 vector3d reference2=diagonal2;
			 reference2.setY(0.0);
			 diagonal1.normalize();
			 diagonal2.normalize();
			 reference1.normalize();
			 reference2.normalize();
			 float d1=diagonal1.dotProduct(reference1);
			 float d2=diagonal2.dotProduct(reference2);
			 if(ramp!=NULL)(*ramp)=false;
			 if(d1<=0.9 || d2<=0.9 ){
				 if(dist1>0){
					 sp.setX(sp.getX()-vn.getX()*(r-dist1));//Segun cual sea el lado por el que colisiona movemos la esfera la distancia necesaria según la normal al plano
					 sp.setY(sp.getY()-vn.getY()*(r-dist1));
					 sp.setZ(sp.getZ()-vn.getZ()*(r-dist1));
				 }else{
					 sp.setX(sp.getX()+vn.getX()*(r-dist2));
					 sp.setY(sp.getY()+vn.getY()*(r-dist2));
					 sp.setZ(sp.getZ()+vn.getZ()*(r-dist2));
				 }
			 }else{
				 if(ramp!=NULL)(*ramp)=true;
				 if(dist1>0){
					 sp.setX(sp.getX());//Segun cual sea el lado por el que colisiona movemos la esfera la distancia necesaria según la normal al plano horizontal de referencia
					 sp.setY(sp.getY()-1*(r-dist1));
					 sp.setZ(sp.getZ());
				 }else{
					 sp.setX(sp.getX());
					 sp.setY(sp.getY()+1*(r-dist2));
					 sp.setZ(sp.getZ());
				 }
			 }
		 }else{
		 
			 if(dist1>0){
				 sp.setX(sp.getX()-vn.getX()*(r-dist1));//Segun cual sea el lado por el que colisiona movemos la esfera la distancia necesaria según la normal al plano
				 sp.setY(sp.getY()-vn.getY()*(r-dist1));
				 sp.setZ(sp.getZ()-vn.getZ()*(r-dist1));
			 }else{
				 sp.setX(sp.getX()+vn.getX()*(r-dist2));
				 sp.setY(sp.getY()+vn.getY()*(r-dist2));
				 sp.setZ(sp.getZ()+vn.getZ()*(r-dist2));
			 }
		 
		}
		 return true;
	 }
	 return false;
}

bool collision::spheresphere(vector3d &c1,float r1,const vector3d& c2,float r2){
	float dist=pointdistance(c1,c2);
	if(dist<=(r1+r2)*(r1+r2)){
		float a=sqrt(dist)-(r1+r2);
		vector3d vec(c2.getX()-c1.getX(),c2.getY()-c1.getY(),c2.getZ()-c1.getZ());
		vec.normalize();
		c1.setX(c1.getX()+vec.getX()*a); 
		c1.setY(c1.getY()+vec.getY()*a);
		c1.setZ(c1.getZ()+vec.getZ()*a);
		
		return true;
	}
	return false;
}

float collision::pointdistance(const vector3d& p1,const vector3d& p2){
	vector3d vec(p2.getX()-p1.getX(),p2.getY()-p1.getY(),p2.getZ()-p1.getZ());
	return /*sqrt*/ (vec.getX()*vec.getX()+vec.getY()*vec.getY()+vec.getZ()*vec.getZ());
}
