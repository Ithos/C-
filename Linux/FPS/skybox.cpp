#include "skybox.h"

skybox::skybox(){
	height=-1;
	for(int i=0;i<6;i++){
		sk[i]=-1;
	}
}

skybox::skybox(unsigned int left,unsigned int back,unsigned int rigth,unsigned int front,unsigned int top,unsigned int bottom,float s){
	sk[SKY_LEFT]=left;
	sk[SKY_BACK]=back;
	sk[SKY_RIGHT]=rigth;
	sk[SKY_FRONT]=front;
	sk[SKY_TOP]=top;
	sk[SKY_BOTTOM]=bottom;
	height=s;
}

skybox::~skybox(){
	glDeleteTextures(6,&sk[0]);//Numero de texturas y direccion del primer elemento
}

void skybox::drawSkybox(){//Esta funcion se basa an la funcion drawCube


	
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);//Mientras dibujamos la skybox deshabilitamos la iluminacion
	//glDisable(GL_DEPTH_TEST);//Esto puede dar problemas a la hora de representar las armas
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,sk[SKY_BACK]);
	glBegin(GL_QUADS);
		//Dibujamos cada cara del cubo por separado
		//Cara frontal
		glTexCoord2f(0,0);
		glVertex3f(height/2,height/2,height/2);//Damos las coordenadas de cada vertice para cada cara
		glTexCoord2f(1,0);
		glVertex3f(-height/2,height/2,height/2);//Hay que tener cuidado en como se dan las coordenadas para que la superficie cubra toda la extension, hay que tener encuenta que la superficie se dibuja de un vertice a otro segun se vayan dando
		glTexCoord2f(1,1);
		glVertex3f(-height/2,-height/2,height/2);
		glTexCoord2f(0,1);
		glVertex3f(height/2,-height/2,height/2);
	glEnd();//Para poder emplear varias texturas en las distintas caras las dibujamos por separado, si tenemos la textura como una unica imagen seguramente podamos dibujar todas las caras a la vez y luego unir los vertices correspondientes de la textura con el cubo, pero conllevaria el ir mapeando los vertices en coordenadas de la imgen a los del cubo en sus coordenadas	
	
	glBindTexture(GL_TEXTURE_2D,sk[SKY_RIGHT]);
	glBegin(GL_QUADS);
		//Cara derecha
		glTexCoord2f(1,0);
		glVertex3f(height/2,height/2,height/2);
		glTexCoord2f(1,1);
		glVertex3f(height/2,-height/2,height/2);
		glTexCoord2f(0,1);
		glVertex3f(height/2,-height/2,-height/2);
		glTexCoord2f(0,0);
		glVertex3f(height/2,height/2,-height/2);
	glEnd();	
	
	glBindTexture(GL_TEXTURE_2D,sk[SKY_FRONT]);
	glBegin(GL_QUADS);
		//Cara trasera
		glTexCoord2f(1,1);
		glVertex3f(height/2,-height/2,-height/2);
		glTexCoord2f(1,0);
		glVertex3f(height/2,height/2,-height/2);
		glTexCoord2f(0,0);
		glVertex3f(-height/2,height/2,-height/2);
		glTexCoord2f(0,1);
		glVertex3f(-height/2,-height/2,-height/2);
	glEnd();	
	
	glBindTexture(GL_TEXTURE_2D,sk[SKY_LEFT]);
	glBegin(GL_QUADS);
		//Cara izquierda
		glTexCoord2f(1,0);
		glVertex3f(-height/2,height/2,-height/2);
		glTexCoord2f(1,1);
		glVertex3f(-height/2,-height/2,-height/2);
		glTexCoord2f(0,1);
		glVertex3f(-height/2,-height/2,height/2);
		glTexCoord2f(0,0);
		glVertex3f(-height/2,height/2,height/2);
	glEnd();	
	
	glBindTexture(GL_TEXTURE_2D,sk[SKY_TOP]);
	glBegin(GL_QUADS);
		//Cara superior
		glTexCoord2f(1,0);
		glVertex3f(height/2,height/2,height/2);
		glTexCoord2f(1,1);
		glVertex3f(height/2,height/2,-height/2);
		glTexCoord2f(0,1);
		glVertex3f(-height/2,height/2,-height/2);
		glTexCoord2f(0,0);
		glVertex3f(-height/2,height/2,height/2);
	glEnd();	
	
	glBindTexture(GL_TEXTURE_2D,sk[SKY_BOTTOM]);
	glBegin(GL_QUADS);
		//Cara inferior
		glTexCoord2f(0,1);
		glVertex3f(-height/2,-height/2,height/2);
		glTexCoord2f(1,1);
		glVertex3f(height/2,-height/2,height/2);
		glTexCoord2f(1,0);
		glVertex3f(height/2,-height/2,-height/2);
		glTexCoord2f(0,0);
		glVertex3f(-height/2,-height/2,-height/2);
	glEnd();
	
	//glEnable(GL_LIGHTING);//GL_LIGHTNING y GL_FOG pueden dar problemas con la iluminación si no estaban previamente activados
	//glEnable(GL_DEPTH_TEST);//Esto lo comentamos si comentamos el disable arriba
	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_FOG); //Si no usamos niebla mejor deshabilitarla despues o comentar esto

}
