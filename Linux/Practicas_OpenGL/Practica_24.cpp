#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
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
#include "animLoader.h"
#include "camera.h"
#include "skybox.h"
#include "text.h"
#include "heightmap.h"

float angle=0.0;
bool mousein=false;
coordinate spherecenter(0.0,0.0,0.0);
coordinate raystart(0.0,0.0,-5.0);//Cuando movamos la camara el punto de inicio del rayo no cambia, pero el centro de la esfera si



int object,textTest;
unsigned int idTexture;
objLoader obj;
text* tex;
std::vector<unsigned int> frames;
int currentFrame=0;

std::vector<std::vector<float> > heightmap;

unsigned int load_texture(const char* filename, bool generate=false){
	
	
	SDL_Surface* image=IMG_Load(filename);//Cargamos la imagen mediante SDL
	
	if(image==NULL){//Chequeamos posibles errores
		std::cout<<"no image loaded"<<std::endl;
		return -1;
	}
	
	//Especificamos el formato al que se convertirá la imagen mediante un pixel format (mirar los apartados en la documentación)
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	
	std::cout<<"converting image"<<std::endl;
	
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
	textTest=obj.loadObject("capsule0.obj");
	
	idTexture=load_texture("texture.jpg",true);
	
	std::vector<int> chars;//Creamos un vector en el que almacenamos la dirección de los archivos de fuente de texto
	char tmp[40];
	for(int i=0;i<58;i++){
		sprintf(tmp,"Font/%d.obj",i);//Almacenamos una dirección de archivo
		int tmp2=obj.loadObject(tmp);//Cargamos el archivo y almacenamos la respuesta en una variable temporal
		chars.push_back(tmp2);//almacenamos la respuesta en el vector
	}
	
	tex=new text(0.9,0.8,chars);
	
	loadHeightmap(heightmap,"heightmap.bmp");
	
	glEnable(GL_LIGHTING);//Habilita la iluminación
	glEnable(GL_LIGHT0);//Habilitamos un foco de luz
	float dif[]={1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);//Marcamos el color e intensidad de la luz dirigida
	//float amb[]={0.2,0.2,0.2,1.0};//?
	//glLightfv(GL_LIGHT0,GL_AMBIENT,amb);//Marcamos el color e intensidad de la luz ambiente
	initSkybox();//Iniciamos la skybox
	loadAnimation(frames,"anim/anim",250,obj);
}

coordinate p1(-5.0,5.0,-5.0);
coordinate p2(5.0,5.0,-5.0);
coordinate p3(5.0,-5.0,-5.0);
coordinate p4(-5.0,-5.0,-5.0);

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
	if(rayplane(0.0,0.0,0.1,raystart.x,raystart.y,raystart.z,0.0,0.0,-1.0,p1,p2,p3,p4)) glColor3f(1.0,0.0,0.0);
	else glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
		glVertex3f(raystart.x,raystart.y,raystart.z);
		glVertex3f(raystart.x+100*0,raystart.y+100*0,raystart.z+100*-1.0);
	glEnd();
    
    glBegin(GL_QUADS);
		glVertex3f(-5.0,5.0,-5.0);
		glVertex3f(5.0,5.0,-5.0);
		glVertex3f(5.0,-5.0,-5.0);
		glVertex3f(-5.0,-5.0,-5.0);
	glEnd();
	
	glColor3f(1.0,1.0,1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,idTexture);
	
	glBegin(GL_QUADS);
		glTexCoord2f(100,0);
		glVertex3f(-200.0,-3.0,200.0);
		glTexCoord2f(0,0);
		glVertex3f(-200.0,-3.0,-200.0);
		glTexCoord2f(0,100);
		glVertex3f(200.0,-3.0,-200.0);
		glTexCoord2f(100,100);
		glVertex3f(200.0,-3.0,200.0);
	glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    glTranslatef(-40.0,-5.0,0.0);
	renderHeightmap(heightmap,0.3,30.0);
	glTranslatef(40.0,5.0,0.0);
    
	glEnable(GL_LIGHTING);
	glCallList(object);//Llamamos a la lista compilada mediante el identificador que nos pasa loadObject
	
	glTranslatef(20.0,0.0,-5.0);
	glCallList(textTest);
	glTranslatef(-20.0,0.0,5.0);
	
	tex->drawText(coordinate(10.0,0,0),coordinate(angle,0,0),"Hi from\nOpenGL\nTest:\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzAB");
	 
	glTranslatef(-20.0,0.0,0.0);
	glCallList(frames[currentFrame]);
	 
	 
	
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
		
		currentFrame++;//Movemos el frame actual
		if(currentFrame>249)currentFrame=0;
		
		display();//Llamamos a la funcion que manda cosas a la pantalla (al buffer de la pantalla)
		SDL_GL_SwapBuffers();//Esto es equivalente al flip de sdl mandamos un buffer a la pantalla y cargamos el otro
		
		
		
		
		angle+=0.5;
		if(angle>=360)angle-=360;
		
		//Vamos con un chequeo de colisiones con la camara y el plano, vamos a emplear para ello la colision entre esfera y plano
		
		coordinate camerapos=camPos();
		sphereplane(camerapos,coordinate(0,0,1),p1,p2,p3,p4,2.0);//Ponemos la esfera justo en frente de la camara
		spheresphere(camerapos,2.0,spherecenter,1.0);
		moveTo(camerapos);
		
		if(1000/30>(SDL_GetTicks()-start)){
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
		}
	}
	SDL_Quit();
	killSkybox();//Eliminamos la skybox
	delete tex;
	return 0;
}
