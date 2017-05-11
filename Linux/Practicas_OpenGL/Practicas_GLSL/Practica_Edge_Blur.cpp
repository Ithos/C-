#include <SDL/SDL.h>
#include "GLee.h"
#include "matrices.h"
#include "shader.h"
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
#include "sceneLoader.h"
#include "modifiedCameraClass.h"




camera cam;
matrices pipeline;
meshLoader* scene;
shader* mainShader;
shader* quadRenderShader;
shader* blurShader;
shader* brightLightShader;
shader* combinedShader;
shader* edgeShader;

unsigned int FBO;//Frame Buffer Object
unsigned int renderTexture,renderTexture2,renderTexture3,depthTexture;
meshClass* quad;//Vamos a dibujar empleando mesh para evitar utilizar el modo directo que esta desactualizado

unsigned int createTexture(int w,int h,bool isDepth=false){
	unsigned int textureId;
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0,(!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT),w,h,0,(isDepth ? GL_DEPTH_COMPONENT : GL_RGBA),GL_FLOAT,NULL);//Comprobamos si hay depth buffer y seleccionamos las propiedades de la textura
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//Filtros de la textura
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	int i;
	i=glGetError();
	if(i!=0){
		std::cout<<"An error ocurred while loading the texture: " << gluErrorString(i) << i << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D,0);
	return textureId;
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

void gaussianBlur(unsigned int tex1,unsigned int tex2,bool& b){
	for(int i=0;i<10;i++){//Esto da problemas si el numero no es par
		glBindFramebuffer(GL_FRAMEBUFFER,FBO);//unimos el buffer como lectura/escritura si queremos solo escritura : GL_WRITE_FRAMEBUFFER : si queremos solo lectura GL_READ_FRAMEBUFFER 
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,b? tex2:tex1,0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,b? tex1:tex2);
		blurShader->useShader();
		glUniform1i(glGetUniformLocation(blurShader->getProgramId(),"texture"),0);
		glUniform2f(glGetUniformLocation(blurShader->getProgramId(),"pixelSize"),1.0/640.0,1.0/480.0);
		glUniform1i(glGetUniformLocation(blurShader->getProgramId(),"isVertical"),(int)b);
		pipeline.updateMatrices(blurShader->getProgramId());//Tenemos que actualizar las matrices siempre antes de dibujar
		quad->draw(blurShader->getProgramId());
		
		b=!b;
	}
}

void gaussianBlurEdge(unsigned int tex1,unsigned int tex2,bool& b){
	for(int i=0;i<4;i++){//Esto da problemas si el numero no es par
		glBindFramebuffer(GL_FRAMEBUFFER,FBO);//unimos el buffer como lectura/escritura si queremos solo escritura : GL_WRITE_FRAMEBUFFER : si queremos solo lectura GL_READ_FRAMEBUFFER 
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,b? tex2:tex1,0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,b? tex1:tex2);
		edgeShader->useShader();
		glUniform1i(glGetUniformLocation(edgeShader->getProgramId(),"texture"),0);
		glUniform2f(glGetUniformLocation(edgeShader->getProgramId(),"pixelSize"),1.0/640.0,1.0/480.0);
		glUniform1i(glGetUniformLocation(edgeShader->getProgramId(),"isVertical"),(int)b);
		pipeline.updateMatrices(edgeShader->getProgramId());//Tenemos que actualizar las matrices siempre antes de dibujar
		quad->draw(edgeShader->getProgramId());
		
		b=!b;
	}
}

void drawTexture(unsigned int tex1){
	//Dibujamos la imagen en la pantalla
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClear(GL_COLOR_BUFFER_BIT);
	quadRenderShader->useShader();
	pipeline.updateMatrices(quadRenderShader->getProgramId());//Tenemos que actualizar las matrices siempre antes de dibujar
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex1);
	glUniform1i(glGetUniformLocation(quadRenderShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(quadRenderShader->getProgramId(),"pixelSize"),1.0/640.0,1.0/480.0);
	quad->draw(quadRenderShader->getProgramId());
	quadRenderShader->delShader();
}

void combineTextures(unsigned int tex1,unsigned int tex2){
	combinedShader->useShader();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex1);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,tex2);
	
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glUniform1i(glGetUniformLocation(combinedShader->getProgramId(),"texture1"),0);
	glUniform1i(glGetUniformLocation(combinedShader->getProgramId(),"texture2"),1);
	glUniform2f(glGetUniformLocation(combinedShader->getProgramId(),"pixelSize"),1.0/640.0,1.0/480.0);
	
	pipeline.updateMatrices(combinedShader->getProgramId());//Tenemos que actualizar las matrices siempre antes de dibujar
	quad->draw(combinedShader->getProgramId());
	
}


void init(){
	//Cambiamos las funciones de OpenGl por las que hemos creado
	glClearColor(0,0,0,1);
	
	pipeline.perspective(50,640.0/480.0,1,1000);
	
   
    //glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    mainShader=new shader("vertex8.vs","fragment8.frag");
    quadRenderShader=new shader("vertex9.vs","fragment9_noef.frag");
    blurShader=new shader("vertex9.vs","fragment9_3.frag");
    brightLightShader=new shader("vertex9.vs","fragment9_4.frag");
    combinedShader=new shader("vertex9.vs","weightedBlend.frag");
    edgeShader=new shader("vertex9.vs","edgeBlur.frag");
    scene=new meshLoader("test.blend");
    
    renderTexture=createTexture(640,480);//Creamos la textura
    renderTexture2=createTexture(640,480);
    renderTexture3=createTexture(640,480);
    depthTexture=createTexture(640,480,true);//Creamos la textura
    glGenFramebuffers(1,&FBO);//Generamos el buffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);//unimos el buffer como lectura/escritura si queremos solo escritura : GL_WRITE_FRAMEBUFFER : si queremos solo lectura GL_READ_FRAMEBUFFER 
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture,0);//Mandamos la textura al buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture,0);//Esto es para la segunda textura, hace que la textura en 3d se vea bien de color 
    int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(i!=GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Framebuffer status is not OK. Status : " << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);//Desactivamos el framebuffer hasta que lo necesitemos para evitar errores
	
	{//Creamos un bloque de código para evitar que las variables que creemos aqui se veán desde fuera
	//Vamos a dibujar quads empleando mesh, rellenando las estructuras de datos correspondientes
		std::vector<unsigned int> indexes;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1er vector
		tmp.position.change(-1.0,1.0,0.0);
		tmp.U=0;
		tmp.V=1;
		vertices.push_back(tmp);
		//2o vector
		tmp.position.change(-1.0,-1.0,0.0);
		tmp.U=0;
		tmp.V=0;
		vertices.push_back(tmp);
		//3er vector
		tmp.position.change(1.0,-1.0,0.0);
		tmp.U=1;
		tmp.V=0;
		vertices.push_back(tmp);
		//4o vector
		tmp.position.change(1.0,1.0,0.0);
		tmp.U=1;
		tmp.V=1;
		vertices.push_back(tmp);
		
		//Incluimos el numero de vertices en orden para cada triángulo que forma el quad
		//indices para el primer triángulo
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		//indices para el segundo triángulo
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		//Creamos un nuevo mesh
		quad=new meshClass(&vertices,&indexes);
	}
	
}
float angle=0.0;

void display(){
	//Dibujamos sobre la textura
	glEnable(GL_DEPTH_TEST);
	pipeline.perspective(50,640.0/480.0,1,1000);
	
	pipeline.matrixMode(VIEW_MATRIX);
	pipeline.loadIdentity();
	cam.control(pipeline);
	cam.updateCamera(pipeline);
	
	pipeline.matrixMode(MODEL_MATRIX);
	pipeline.translate(0.0,0.0,-3.0);
	glUniform3f(glGetUniformLocation(mainShader->getProgramId(),"lightPos"),0,1,2);
	glUniform3f(glGetUniformLocation(mainShader->getProgramId(),"cameraPosition"),cam.getLocation().getX(),cam.getLocation().getY(),cam.getLocation().getZ());
	mainShader->useShader();
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);//Movemos el clear para que borre el frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture,0);
	glClearColor(0.0,0.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	pipeline.updateMatrices(mainShader->getProgramId());//Tenemos que actualizar las matrices siempre antes de dibujar
	scene->draw(mainShader->getProgramId());
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	mainShader->delShader();
	
	//Filtro de difuminado
	glDisable(GL_DEPTH_TEST);
	pipeline.loadIdentity();
	pipeline.ortho(-1,1,-1,1,-1,1);
	
	brightLightShader->useShader();
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture3,0);
	glClear(GL_COLOR_BUFFER_BIT);
	pipeline.updateMatrices(brightLightShader->getProgramId());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,renderTexture);
	glUniform1i(glGetUniformLocation(brightLightShader->getProgramId(),"texture"),0);
	glUniform2f(glGetUniformLocation(brightLightShader->getProgramId(),"pixelSize"),1.0/640.0,1.0/480.0);
	quad->draw(brightLightShader->getProgramId());
	
	bool b=true;//Este bool nos dirá en que buffer está la imagen
	gaussianBlur(renderTexture3,renderTexture2,b);
	gaussianBlurEdge(renderTexture,renderTexture2,b);
	
	combineTextures(renderTexture3,renderTexture);
	//drawTexture(renderTexture3);
	
	//angle+=3.0;
	//if(angle>=360.0)angle-=360;
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
				cam.mouseIn(true);
				SDL_ShowCursor(SDL_DISABLE);
			}
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_p){
					cam.mouseIn(false);
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
	SDL_Quit();
	delete scene;
	delete quad;
	delete mainShader;
	delete quadRenderShader;
	delete blurShader;
	delete brightLightShader;
	delete combinedShader;
	return 0;
}
