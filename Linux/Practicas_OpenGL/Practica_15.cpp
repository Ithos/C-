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
#include "objLoader.h"
#include "camera.h"
#include "skybox.h"

float angle=0.0;
bool mousein=false;



int object;

objLoader obj;

void init(){
	glClearColor(0.0,0.0,0.0,1.0);//r,g,b,alpha //Este sera el color del fondo, es decir el color al que se reinicializara el buffer de color
	glMatrixMode(GL_PROJECTION);//Indica que representar, le decimos que vamos a dar los parametros de la proyeccion
	glLoadIdentity();//Resetea la matriz de proyeccion a la identidad
	gluPerspective(45,640.0/480.0,1.0,500.0);//angulo, ancho/alto, distancia al plano de representacion mas cercano, distanca al plano de representacion mas lejano //Tambien se puede usar glOrtho de esta forma multiplicamos la matriz actual por una matriz ortografica, que simboliza una proyeccion paralela el (efecto que se obtendria no seria realista para imagenes 3d)
	glMatrixMode(GL_MODELVIEW);//Una vez terminamos de dar parametros volvemos al modo de vista del modelo	
	glShadeModel(GL_SMOOTH);//Esto nos permite decir como queremos que varien los colores, por defecto si un objeto tiene varios colores el color cambia de forma gradual, si queremos que el objeto sea del ultimo color que dimos hay q usar la opcin GL_FLAT
	//GL_SMOOTH es la opcion por defecto
	glEnable(GL_DEPTH_TEST);//Esta funcion permite habilitar varias opciones de openGL, esta opcion asigna un array a cada pixel de la pantalla para manejar la profundidad
	object=obj.loadObject("test2.obj");
	glEnable(GL_LIGHTING);//Habilita la iluminación
	glEnable(GL_LIGHT0);//Habilitamos un foco de luz
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//Esta funcion nos permite fijar los parametros de las transparencias //mediante esta opcion cambiamos el color del objeto de forma lineal segun el color de los objetos que haya detras o del color del fondo
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE,GL_LINEAR);//Estas tres lineas son para añadir niebla mediante un algoritmo lineal
	glFogf(GL_FOG_START,4.0);
	glFogf(GL_FOG_END,8.0);
	//glFogi(GL_FOG_MODE,GL_EXP);//Esto es para añadir niebla mediante un algoritmo exponencial
	//glFogf(GL_FOG_DENSITY,0.6);
	//glFogi(GL_FOG_MODE,GL_EXP2);//Esto es para añadir niebla mediante un algoritmo exponencial doble (o algo asi)
	//glFogf(GL_FOG_DENSITY,0.4);
	float color[]={0.0,0.0,0.0,1.0};
	glFogfv(GL_FOG_COLOR,color);
	float dif[]={1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);//Marcamos el color e intensidad de la luz dirigida
	float amb[]={0.2,0.2,0.2,1.0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,amb);//Marcamos el color e intensidad de la luz ambiente
	initSkybox();//Iniciamos la skybox
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Le decimos que buffers reinicializar
	glLoadIdentity();//Con esto reseteamos la matriz a la identidad, de esta forma evitamos que la siguiente traslacion se aplique continuamente y conseguimos que se aplique solo una vez
	control(0.2,0.2,mousein);//Funcion de la camara
	drawSkybox(100.0);//Dibujamos la skybox
	updateCamera();
	float pos[]={-2.0,2.0,-3.0,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);//Indicamos la posicion del focode luz
	glRotatef(angle,1.0,1.0,1.0);//Mediante esta funcion rotamos en un eje //angulo, ejes(x,y,z)
	glCallList(object);//Llamamos a la lista compilada mediante el identificador que nos pasa loadObject
	
}

int main(int argc,char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);
	bool run=true;
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
			}
		}
		
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
