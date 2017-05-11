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

float angle=0.0;

struct coordinate{//Aqui almacenamos las coordenadas de los vertices
	float x,y,z;
	coordinate(float a,float b,float c) : x(a),y(b),z(c) {};//Constructor con lista de inicializacion
};

struct face{//Aqui almacenamos diversos parametros de las caras
	int facenum;//Almacena el numero de la cara
	bool four;//Indica si la cara esta formada por cuadrdos o por triangulos
	int faces[4];//Almacena las coordenadas de los vertices de la cara
	face(int facen,int f1,int f2,int f3) : facenum(facen) {//Sobrecargamos el constructor de tal forma que si la cara esta formada por triangulos sera este constructor el que salte
		faces[0]=f1;
		faces[1]=f2;
		faces[2]=f3;
		four=false;
	};
	face(int facen,int f1,int f2,int f3,int f4) : facenum(facen) {
		faces[0]=f1;
		faces[1]=f2;
		faces[2]=f3;
		faces[3]=f4;
		four=true;//Si la cara esta formada por cuadrados este es el constructor que se emplea y entoces ponemos four a true
	};
};

int loadObject(const char* filename){
	std::vector<std::string*> coord;//Este vector lo lee todo como chars, por lo que practicamente es capaz de leerlo todo
	std::vector<coordinate*> vertex;//Aqui almacenamos los vertices
	std::vector<face*> faces;//En este vector almacenamos las caras
	std::vector<coordinate*> normals;//Aqui almacenamos los vectores normales a las caras
	std::ifstream in(filename);
	if(!in.is_open()){
		std::cout<<"File not found."<<std::endl;
		return -1;
	}
	char c[256];//Creamos un buffer en el que almacenar lo que leemos
	while(!in.eof()){
		in.getline(c,256);
		coord.push_back(new std::string(c));
	}
	for(int i;i<coord.size();i++){
		if((*coord[i])[0]=='#'){//Si el primer simbolo de la linea es # la ignoramos ya que es un comentario
			
		}else if((*coord[i])[0]=='v'&&(*coord[i])[1]==' '){
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);
			vertex.push_back(new coordinate(tmpx,tmpy,tmpz));
		}else if((*coord[i])[0]=='v'&&(*coord[i])[1]=='n'){
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
			normals.push_back(new coordinate(tmpx,tmpy,tmpz));
		}else if((*coord[i])[0]=='f'){
			int a,b,c,d,e;
			if(count(coord[i]->begin(),coord[i]->end(),' ')==4){//Count nos devuelve el numero de elementos en rango
				sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);//Almacenamos las variables
				faces.push_back(new face(b,a,c,d,e));
			}else{//En caso de que la cara este formada por triangulos tendra un vertice menos, en ese caso llamamos al otro constructor
				sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
				faces.push_back(new face(b,a,c,d));
			}
		}
	}
	//Aqui dibujamos
	int num;
	num=glGenLists(1);//Cada vez que llamamos a GenLists el numero que obtenemos se incrementa en el numero de unidades que indicamos
	glNewList(num,GL_COMPILE);//Creamos una nueva lista y de momento solo compilamos
		for(int i=0;i<faces.size();i++){
			if(faces[i]->four){//Si la cara esta formada por un cuadrado/rectangulo
				glBegin(GL_QUADS);
					glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);//Creamos un vector normal a la cara
					glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);//Creamos vertices para las caras
					glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
					glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
					glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
				glEnd();
			}else{//En caso de que sea un triangulo
				glBegin(GL_TRIANGLES);
					glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);//Creamos un vector normal a la cara
					glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);//Creamos vertices para las caras
					glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
					glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
				glEnd();
			}
		}
		glEndList();
		
		
		for(int i;i<coord.size();i++)delete coord[i];//Cuando terminamos borramos el vector coord
		for(int i;i<faces.size();i++)delete faces[i];//Tambien borramos el vector faces
		for(int i;i<normals.size();i++)delete normals[i];//Tambien borramos el vector normals
		for(int i;i<vertex.size();i++)delete vertex[i];//Tambien borramos el vector vertex
		return num;//Devolvemos un identificador para la lista
	
}

int cube;

void init(){
	glClearColor(0.0,0.0,0.0,1.0);//r,g,b,alpha //Este sera el color del fondo, es decir el color al que se reinicializara el buffer de color
	glMatrixMode(GL_PROJECTION);//Indica que representar, le decimos que vamos a dar los parametros de la proyeccion
	glLoadIdentity();//Resetea la matriz de proyeccion a la identidad
	gluPerspective(45,640.0/480.0,1.0,500.0);//angulo, ancho/alto, distancia al plano de representacion mas cercano, distanca al plano de representacion mas lejano //Tambien se puede usar glOrtho de esta forma multiplicamos la matriz actual por una matriz ortografica, que simboliza una proyeccion paralela el (efecto que se obtendria no seria realista para imagenes 3d)
	glMatrixMode(GL_MODELVIEW);//Una vez terminamos de dar parametros volvemos al modo de vista del modelo	
	glShadeModel(GL_SMOOTH);//Esto nos permite decir como queremos que varien los colores, por defecto si un objeto tiene varios colores el color cambia de forma gradual, si queremos que el objeto sea del ultimo color que dimos hay q usar la opcin GL_FLAT
	//GL_SMOOTH es la opcion por defecto
	glEnable(GL_DEPTH_TEST);//Esta funcion permite habilitar varias opciones de openGL, esta opcion asigna un array a cada pixel de la pantalla para manejar la profundidad
	cube=loadObject("test.obj");
	glEnable(GL_LIGHTING);//Habilita la iluminación
	glEnable(GL_LIGHT0);//Habilitamos un foco de luz
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE,GL_LINEAR);//Estas tres lineas son para añadir niebla mediante un algoritmo lineal
	glFogf(GL_FOG_START,1.0);
	glFogf(GL_FOG_END,5.0);
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
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Le decimos que buffers reinicializar
	glLoadIdentity();//Con esto reseteamos la matriz a la identidad, de esta forma evitamos que la siguiente traslacion se aplique continuamente y conseguimos que se aplique solo una vez
	float pos[]={-2.0,2.0,-3.0,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);//Indicamos la posicion del focode luz
	glTranslatef(0.0,0.0,-5.0);//Cada vez que lo llamemos multiplica la matriz actual por una de traslacion que indiquemos moviendo cosas en uno o varios ejes
	glRotatef(angle,1.0,1.0,1.0);//Mediante esta funcion rotamos en un eje //angulo, ejes(x,y,z)
	//Las traslaciones de arriba se aplican a todas las imagenes a partir de su ejecucion //llamamos a la lista mediante su identificador
	glCallList(cube);//Llamamos a la lista compilada mediante el identificador que nos pasa loadObject
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
	return 0;
}
