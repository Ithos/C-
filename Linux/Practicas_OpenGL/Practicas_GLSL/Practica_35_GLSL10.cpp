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
#include "modifiedCameraClass2.h"




camera cam;
matrices pipeline;
meshLoader* scene;
shader* mainShader;
shader* quadRenderShader;
shader* skyboxShader;
shader* reflectShader;

unsigned int cubemap;

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

SDL_Surface* loadTexture(std::string& filename){
	unsigned int num;
	glGenTextures(1,&num);
	SDL_Surface* image=IMG_Load(filename.c_str());//Cargamos la imagen mediante SDL
	
	if(image==NULL){//Chequeamos posibles errores
		std::cout<<"no image loaded"<<std::endl;
		return 0;
	}
	
	//Especificamos el formato al que se convertirá la imagen mediante un pixel format (mirar los apartados en la documentación)
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	
	//Convertimos la imagen a un formato independiente del archivo
	SDL_Surface* img2=SDL_ConvertSurface(image,&form,SDL_SWSURFACE);
	
	SDL_FreeSurface(image);
	 
	return img2;
	
}

unsigned int loadCubemap(std::string *filenames){
	//Las texturas tienen que introducirse en el orden: derecha,izquierda,arriba,abajo,cerca,lejos
	unsigned int tex;
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	for(int i=0;i<6;i++){
		SDL_Surface* img=loadTexture(filenames[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img->pixels);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		SDL_FreeSurface(img);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	return tex;
}


unsigned int FBO;//Frame Buffer Object
unsigned int renderTexture,depthTexture;
meshClass* quad;//Vamos a dibujar empleando mesh para evitar utilizar el modo directo que esta desactualizado
meshClass* aCube;//Aqui vamos a dibujar un cubo para la skybox

void init(){
	//Cambiamos las funciones de OpenGl por las que hemos creado
	glClearColor(0,0,0,1);
	
	pipeline.perspective(50,640.0/480.0,1,1000);
	
   
    //glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    mainShader=new shader("vertex8.vs","fragment8.frag");
    quadRenderShader=new shader("vertex9.vs","fragment9.frag");
    skyboxShader=new shader("skyboxShader.vs","skyboxShader.frag");
    reflectShader=new shader("reflection.vs","reflection.frag");
    scene=new meshLoader("test.blend");
    
    renderTexture=createTexture(640,480);//Creamos la textura
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
	
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		
		//Definimos vertices
		tmp.position.change(-1.0,1.0,1.0);
		tmp.color.change(1.0,0.0,0.0);
		vertices.push_back(tmp);
		
		tmp.position.change(-1.0,-1.0,1.0);
		tmp.color.change(0.0,1.0,0.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,-1.0,1.0);
		tmp.color.change(0.0,0.0,1.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,1.0,1.0);
		tmp.color.change(0.0,1.0,1.0);
		vertices.push_back(tmp);
		
		tmp.position.change(-1.0,1.0,-1.0);
		tmp.color.change(1.0,0.0,1.0);
		vertices.push_back(tmp);
		
		tmp.position.change(-1.0,-1.0,-1.0);
		tmp.color.change(1.0,1.0,0.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,-1.0,-1.0);
		tmp.color.change(1.0,1.0,1.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,1.0,-1.0);
		tmp.color.change(0.0,0.0,0.0);
		vertices.push_back(tmp);
		
		//Definimos caras (hay que definir los vertices de las caras en sentido de las agujas del reloj)
		
		//Cara frontal
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
		
		//Cara derecha
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(6);
		
		indices.push_back(3);
		indices.push_back(7);
		indices.push_back(6);
		
		//Cara trasera
		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);
		
		indices.push_back(4);
		indices.push_back(7);
		indices.push_back(6);
		
		//Cara izquierda
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(5);
		
		indices.push_back(0);
		indices.push_back(4);
		indices.push_back(5);
		
		//Cara superior
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(4);
		
		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(7);
		
		//Cara inferior
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(6);
		
		indices.push_back(1);
		indices.push_back(5);
		indices.push_back(6);
		
		aCube=new meshClass(&vertices,&indices);
		
	}
	
	std::string filenames[6];
	filenames[0]="skybox/rightLarge.bmp";
	filenames[1]="skybox/leftLarge.bmp";
	filenames[2]="skybox/upLarge.bmp";
	filenames[3]="skybox/bottomLarge.bmp";
	filenames[4]="skybox/frontLarge.bmp";
	filenames[5]="skybox/backLarge.bmp";
	
	cubemap=loadCubemap(filenames);
	
}
float angle=0.0;

void display(){
	
	pipeline.perspective(50,640.0/480.0,0.5,1000);

	pipeline.loadIdentity();
	cam.control(pipeline);
	
	glClearColor(0.0,0.5,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST);//Deshabilitamos la profundidad para la skybox
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	pipeline.matrixMode(MODEL_MATRIX);
	skyboxShader->useShader();
	
	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);
	glUniform1i(glGetUniformLocation(skyboxShader->getProgramId(),"cubemap"),0);
	
	
	
	pipeline.updateMatrices(skyboxShader->getProgramId());//Tenemos que actualizar las matrices siempre antes de dibujar
	aCube->draw(skyboxShader->getProgramId());
	skyboxShader->delShader();
	
	cam.updateCamera(pipeline);
	
	glEnable(GL_DEPTH_TEST);//Habilitamos al profundidad
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	pipeline.matrixMode(MODEL_MATRIX);
	pipeline.translate(0.0,0.0,-3.0);
	//glUniform3f(glGetUniformLocation(mainShader->getProgramId(),"lightPos"),0,1,2);
	reflectShader->useShader();
	
	glUniform3f(glGetUniformLocation(reflectShader->getProgramId(),"cameraPosition"),cam.getLocation().getX(),cam.getLocation().getY(),cam.getLocation().getZ());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap);
	glUniform1i(glGetUniformLocation(reflectShader->getProgramId(),"cubemap"),0);
	
	
	
	pipeline.updateMatrices(reflectShader->getProgramId());//Tenemos que actualizar las matrices siempre antes de dibujar
	scene->draw(reflectShader->getProgramId());
	reflectShader->delShader();
	
	

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
	delete aCube;
	delete skyboxShader;
	delete reflectShader;
	return 0;
}
