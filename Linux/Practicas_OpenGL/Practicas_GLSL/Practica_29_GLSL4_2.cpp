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
#include "objLoader.h"


bool mousein=false;

void loadFile(const char* fn,std::string &str){//Esta función carga un archivo y lo almacena en str
	std::ifstream in(fn);
	if(!in.is_open()){
		std::cout << "The file " << fn << "could not be opened." << std::endl;
		return;
	}
	char tmp[1000];
	while(!in.eof()){
		in.getline(tmp,1000);
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

unsigned int load_texture(const char* filename, bool generate=false){
	
	
	SDL_Surface* image=IMG_Load(filename);//Cargamos la imagen mediante SDL
	
	if(image==NULL){//Chequeamos posibles errores
		std::cout<<"no image loaded"<<std::endl;
		return -1;
	}
	
	//Especificamos el formato al que se convertirá la imagen mediante un pixel format (mirar los apartados en la documentación)
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	
	//Convertimos la imagen a un formato independiente del archivo
	SDL_Surface* img2=SDL_ConvertSurface(image,&form,SDL_SWSURFACE);
	 
	 if(img2==NULL){//Chequeamos posibles errores
		std::cout<<"error in image format conversion"<<std::endl;
		return -1;
	}
	
	
	unsigned int id;//Creamos el identificador de la textura
	glGenTextures(1,&id);//Esta funcion nos pide el numero de texturas y la direccion de memoria de los identificadores //El segundo parametro dberia ser la dir de mem a un array con los ids de todas las texturas, en nuestro caso solo necesitamos 1 id
	glBindTexture(GL_TEXTURE_2D,id);//Indicamos el tipo de textura y el identificador
	if(generate) gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,image->w,image->h,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);//Con esto generamos los mipmaps en tiempo de ejecucion, si se quiere mejorar hay que generarlos previamente y cargarlos de forma distinta, mirar documentación
	else glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->w,image->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);//Cargamos la textura dando diversos parametros y le pasamos el mapa de pixeles
	if(generate)glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//Esto es por si se carga un mipmap
	else glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//Ajustamos el filtro minimo
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Ajustamos el filtro nose si maximo o magnificante o como sea
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	SDL_FreeSurface(image);//Eliminamos el surface pq los datos ya se han copiado
	SDL_FreeSurface(img2);
	return id;//Devolvemos el id de la textura
}

unsigned int myImg,myImg2;

void init(){
	glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
        gluPerspective(50,640.0/480.0,1,1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    initShader("vertex4_2.vs","fragment4_2.frag");
    myImg=load_texture("texture.jpg",true);
}

void display(){
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Le decimos que buffers reinicializar
	glLoadIdentity();//Con esto reseteamos la matriz a la identidad, de esta forma evitamos que la siguiente traslacion se aplique continuamente y conseguimos que se aplique solo una vez
	control(0.2,0.2,mousein);//Funcion de la camara
	updateCamera();
	
	glUniform3f(glGetUniformLocation(program,"lightPos"),0.0,0.0,0.0);//Damos al shader propiedades y posiciones de la luz y del material
	
	/*glUniform3f(glGetUniformLocation(program,"mAmbient"),0.2,0.2,0.2);
	glUniform3f(glGetUniformLocation(program,"mDiffuse"),0.5,0.5,0.8);
	glUniform3f(glGetUniformLocation(program,"mSpecular"),0.8,0.8,0.8);*/
	
	glUniform3f(glGetUniformLocation(program,"lAmbient"),0.2,0.2,0.2);
	glUniform3f(glGetUniformLocation(program,"lDiffuse"),0.5,0.7,0.5);
	glUniform3f(glGetUniformLocation(program,"lSpecular"),1.0,1.0,1.0);
	
	glUniform1f(glGetUniformLocation(program,"shininess"),30.0);
	
	glActiveTexture(GL_TEXTURE0);//Indicamos que la siguiente textura activa tendrá el indicador 0
	glBindTexture(GL_TEXTURE_2D,myImg);
	
	glUniform1i(glGetUniformLocation(program,"img"),0);//Con esto indicamos la textura que queremos cargar
	
    glBegin(GL_QUADS);
		glTexCoord2f(1,0); //Si se quieren emplear varias texturas directamente con openGl se puede emplear glMultiTexCoord2f              
        glVertex3f(-1,1,-4);    
        glTexCoord2f(0,0);
        glVertex3f(-1,-1,-4);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,-4);
        glTexCoord2f(1,1);
        glVertex3f(1,1,-4);
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
