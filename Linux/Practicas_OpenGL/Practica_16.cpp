#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>//Contiene funciones para trabajar con rangos de elementos
#include <fstream>
#include <iostream>
#include <cstdio>
#include "rayTracing.h"
#include "objLoader.h"
#include "camera.h"
#include "skybox.h"

float angle=0.0;
bool mousein=false;
coordinate spherecenter(0.0,0.0,0.0);
coordinate raystart(0.0,0.0,-5.0);//Cuando movamos la camara el punto de inicio del rayo no cambia, pero el centro de la esfera si



int object;

objLoader obj;

void init(){
	glClearColor(0.5,0.5,0.5,1.0);//r,g,b,alpha //Este sera el color del fondo, es decir el color al que se reinicializara el buffer de color
	glMatrixMode(GL_PROJECTION);//Indica que representar, le decimos que vamos a dar los parametros de la proyeccion
	glLoadIdentity();//Resetea la matriz de proyeccion a la identidad
	gluPerspective(45,640.0/480.0,1.0,500.0);//angulo, ancho/alto, distancia al plano de representacion mas cercano, distanca al plano de representacion mas lejano //Tambien se puede usar glOrtho de esta forma multiplicamos la matriz actual por una matriz ortografica, que simboliza una proyeccion paralela el (efecto que se obtendria no seria realista para imagenes 3d)
	glMatrixMode(GL_MODELVIEW);//Una vez terminamos de dar parametros volvemos al modo de vista del modelo	
	glShadeModel(GL_SMOOTH);//Esto nos permite decir como queremos que varien los colores, por defecto si un objeto tiene varios colores el color cambia de forma gradual, si queremos que el objeto sea del ultimo color que dimos hay q usar la opcin GL_FLAT
	//GL_SMOOTH es la opcion por defecto
	glEnable(GL_DEPTH_TEST);//Esta funcion permite habilitar varias opciones de openGL, esta opcion asigna un array a cada pixel de la pantalla para manejar la profundidad
	object=obj.loadObject("sphere.obj");
	glEnable(GL_LIGHTING);//Habilita la iluminación
	glEnable(GL_LIGHT0);//Habilitamos un foco de luz
	float dif[]={1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);//Marcamos el color e intensidad de la luz dirigida
	//float amb[]={0.2,0.2,0.2,1.0};//?
	//glLightfv(GL_LIGHT0,GL_AMBIENT,amb);//Marcamos el color e intensidad de la luz ambiente
	initSkybox();//Iniciamos la skybox
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Le decimos que buffers reinicializar
	glLoadIdentity();//Con esto reseteamos la matriz a la identidad, de esta forma evitamos que la siguiente traslacion se aplique continuamente y conseguimos que se aplique solo una vez
	control(0.2,0.2,mousein);//Funcion de la camara
	drawSkybox(100.0);//Dibujamos la skybox
	updateCamera();
	float pos[]={-2.0,1.0,-2.0,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);//Indicamos la posicion del focode luz
	//glRotatef(angle,1.0,1.0,1.0);//Mediante esta funcion rotamos en un eje //angulo, ejes(x,y,z)
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);//Dejo la niebla puesta en skybox
	if(raysphere(spherecenter.x,spherecenter.y,spherecenter.z,0.0,0.0,1.0,raystart.x,raystart.y,raystart.z,1.0)) glColor3f(1.0,0.0,0.0);
	else glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
		glVertex3f(raystart.x,raystart.y,raystart.z);
		glVertex3f(raystart.x+100*0,raystart.y+100*0,raystart.z+100*1.0);
	glEnd();
    
	glEnable(GL_LIGHTING);
	
	glCallList(object);//Llamamos a la lista compilada mediante el identificador que nos pasa loadObject
	
	glColor3f(1.0,1.0,1.0);
	
}

int main(int argc,char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);
	bool run=true;
	bool b[4]={0,0,0,0};
	Uint32 start;
	SDL_Event event;
	
	init();//LLamamos a la funcion de inicializacion que hemos creado arriba
	
	while(run){
		start=SDL_GetTicks();
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_MOUSEBUTTONDOWN){
				mousein=true;
				SDL_ShowCursor(SDL_DISABLE);
			}
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_p){
					mousein=false;
					SDL_ShowCursor(SDL_ENABLE);
				}
				if(event.key.keysym.sym==SDLK_ESCAPE){
					run=false;
				}
				if(event.key.keysym.sym==SDLK_UP){
					b[0]=1;
				}
				if(event.key.keysym.sym==SDLK_DOWN){
					b[1]=1;
				}
				if(event.key.keysym.sym==SDLK_RIGHT){
					b[2]=1;
				}
				if(event.key.keysym.sym==SDLK_LEFT){
					b[3]=1;
				}
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP){
					b[0]=0;
				}
				if(event.key.keysym.sym==SDLK_DOWN){
					b[1]=0;
				}
				if(event.key.keysym.sym==SDLK_RIGHT){
					b[2]=0;
				}
				if(event.key.keysym.sym==SDLK_LEFT){
					b[3]=0;
				}
			}
		}
		
		if(b[0])raystart.y+=0.3;
		if(b[1])raystart.y-=0.3;
		if(b[2])raystart.x+=0.3;
		if(b[3])raystart.x-=0.3;
		
		display();//Llamamos a la funcion que manda cosas a la pantalla (al buffer de la pantalla)
		SDL_GL_SwapBuffers();//Esto es equivalente al flip de sdl mandamos un buffer a la pantalla y cargamos el otro
		
		angle+=0.5;
		if(angle>=360)angle-=360;
		
		if(1000/30>(SDL_GetTicks()-start)){
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
		}
	}
	SDL_Quit();
	killSkybox();//Eliminamos la skybox
	return 0;
}
