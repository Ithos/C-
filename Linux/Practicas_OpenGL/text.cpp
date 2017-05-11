#include "text.h"

text::text(float w,float h,std::vector<int>& ch){
	
	width=w;
	height=h;
	characters=ch;
	
}

void text::drawText(coordinate pos, coordinate rot, const char* tex){
	
	glPushMatrix();//Guardamos la matriz de rotación que teníamos
		glTranslatef(pos.x,pos.y,pos.z);//Trasladamos el cursor a la posición en la que vamos a escribir
		glRotatef(rot.x,1.0,0.0,0.0);//Rotamos en los ejes x, y, z
		glRotatef(rot.y,0.0,1.0,0.0);
		glRotatef(rot.z,0.0,0.0,1.0);
		int g=0;//Con esto contamos el número de caracteres representados en la línea
		for(int i=0;i<strlen(tex);i++){
			if(tex[i]!='\n'){
				g++;
				if(tex[i]>=65 && tex[i]<=122){
					glCallList(characters.at((int)tex[i]-65));//Convertimos de código ascii a orden alfabético
				}
				glTranslatef(width,0,0);//Avanzamos el cursor a la siguiente posición
			}else{
				 glTranslatef(-g*width,-height,0);//Pasamos a la siguiente línea
                 g=0;//Reseteamos el contador
			}
		}
	glPopMatrix();//Recuperamos la matriz de rotación inicial
	
}
