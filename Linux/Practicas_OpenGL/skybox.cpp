#include "skybox.h"

enum{SKY_LEFT=0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM};//Con enum damos valor al primer elemento y todos se declaran como constantes cuyo valor es el del anterior +1
unsigned int skybox[6];//Aqui almacenaremos los identificadores de las texturas


void initSkybox(){
	skybox[SKY_LEFT]=loadTexture("leftLarge.bmp");
	skybox[SKY_BACK]=loadTexture("backLarge.bmp");
	skybox[SKY_RIGHT]=loadTexture("rightLarge.bmp");
	skybox[SKY_FRONT]=loadTexture("frontLarge.bmp");
	skybox[SKY_TOP]=loadTexture("upLarge.bmp");
	skybox[SKY_BOTTOM]=loadTexture("bottomLarge.bmp");
}

void killSkybox(){
	glDeleteTextures(6,&skybox[0]);//Numero de texturas y direccion del primer elemento
}

void drawSkybox(float height){//Esta funcion se basa an la funcion drawCube


	
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);//Mientras dibujamos la skybox deshabilitamos la iluminacion
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_BACK]);
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
	
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_RIGHT]);
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
	
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_FRONT]);
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
	
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_LEFT]);
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
	
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_TOP]);
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
	
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_BOTTOM]);
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
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_FOG);

}

unsigned int loadTexture(const char* filename){
	
	SDL_Surface* image=SDL_LoadBMP(filename);//Cargamos la imagen mediante SDL
	unsigned int id;//Creamos el identificador de la textura
	glGenTextures(1,&id);//Esta funcion nos pide el numero de texturas y la direccion de memoria de los identificadores //El segundo parametro dberia ser la dir de mem a un array con los ids de todas las texturas, en nuestro caso solo necesitamos 1 id
	glBindTexture(GL_TEXTURE_2D,id);//Indicamos el tipo de textura y el identificador
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->w,image->h,0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,image->pixels);//Cargamos la textura dando diversos parametros y le pasamos el mapa de pixeles
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//Ajustamos el filtro minimo
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Ajustamos el filtro nose si maximo o magnificante o como sea
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);//Estas dos funciones son para que no se vea el cubo
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);//No se pq no terminana de funcionar, puede ser por la textura
	SDL_FreeSurface(image);//Eliminamos el surface pq los datos ya se han copiado
	return id;//Devolvemos el id de la textura
}
