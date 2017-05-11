#include <SDL/SDL.h>
#include "GLee.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>//Contiene funciones para trabajar con rangos de elementos
#include <fstream>
#include <iostream>
#include <cstdio>
#include "camera.h"


bool mousein=false;

void loadFile(const char* fn,std::string &str){//Esta función carga un archivo y lo almacena en str
	std::ifstream in(fn);
	if(!in.is_open()){
		std::cout << "The file " << fn << "could not be opened." << std::endl;
		return;
	}
	char tmp[300];
	while(!in.eof()){
		in.getline(tmp,300);
		str+=tmp;
		str+='\n';
	}
	
}

unsigned int loadShader(std::string &source,unsigned int mode){//Esta función compila los shaders y emite los errores de compilación
	unsigned int id;
	id=glCreateShader(mode);
	
	const char* csource=source.c_str();//Convertimos el código fuente de source a strings con el estilo de c
	
	glShaderSource(id,1,&csource,NULL);//introducimos el código
	glCompileShader(id);//Compilamos el shader
	
	char error[1000];
	glGetShaderInfoLog(id,1000,NULL,error);
	std::cout << "Compile status:\n" << error << std::endl;
	
	return id;
}

unsigned int vs,fs,program;//Estas variables son globales por simplicidad, si se puede deben cambiarse

void initShader(const char* vname, const char* fname){//Esta función crea el proyecto de shaders y los linka, en esta función solo empleamos dos archivos, pero si hay mas habrá que incluirlos como parámetros
	std::string source;
	loadFile(vname,source);//Cargamos el código fuente
	vs=loadShader(source,GL_VERTEX_SHADER);//cargamos el shader y almacenamos su id
	source="";//Borramos source
	loadFile(fname,source);
	fs=loadShader(source,GL_FRAGMENT_SHADER);
	
	program=glCreateProgram();//Creamos un programa vacío
	glAttachShader(program,vs);//agregamos shader
	glAttachShader(program,fs);
	
	glLinkProgram(program);//Linkamos los archivos compilados
	glUseProgram(program);//Usamos el programa
	
	
}

void clean(){//Borramos los shaders y el programa, es por esto que necesitamos variables globales
	glDetachShader(program,vs);
	glDetachShader(program,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
}



void init(){
	glClearColor(0,0,0,1);
        glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(50,640.0/480.0,1,1000);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
        initShader("vertex.vs","fragment.frag");
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Le decimos que buffers reinicializar
	glLoadIdentity();//Con esto reseteamos la matriz a la identidad, de esta forma evitamos que la siguiente traslacion se aplique continuamente y conseguimos que se aplique solo una vez
	control(0.2,0.2,mousein);//Funcion de la camara
	updateCamera();
	
	glBegin(GL_TRIANGLES);
		glVertex3f(0,1,-4);
        glVertex3f(-1,-1,-4);
        glVertex3f(1,-1,-4);
    glEnd();
	
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
		
		display();//Llamamos a la funcion que manda cosas a la pantalla (al buffer de la pantalla)
		SDL_GL_SwapBuffers();//Esto es equivalente al flip de sdl mandamos un buffer a la pantalla y cargamos el otro
		
		
		if(1000/30>(SDL_GetTicks()-start)){
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
		}
	}
	clean();
	SDL_Quit();
	return 0;
}
