#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

float angle=0.0;

void DrawCube(float height){
	float difamb[]={0.2,0.4,1.0};
	glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,difamb);//Mediante esta funcion creamos el material y especificamos sus propiedades, el material se emplea desde que se crea hasta que se crea otro
		//Dibujamos cada cara del cubo por separado
		//Cara frontal
		glNormal3f(0.0,0.0,1.0);//Definimos los vectores normales a la superficie para cada cara
		//glColor3f(1.0,0.0,0.0); //Deshabilitamos los colores para hacer pruebas con materiales, si usamos enable color material deberiamos habilitarlos
		glVertex3f(height/2,height/2,height/2);//Damos las coordenadas de cada vertice para cada cara
		glVertex3f(-height/2,height/2,height/2);//Hay que tener cuidado en como se dan las coordenadas para que la superficie cubra toda la extension, hay que tener encuenta que la superficie se dibuja de un vertice a otro segun se vayan dando
		glVertex3f(-height/2,-height/2,height/2);
		glVertex3f(height/2,-height/2,height/2);
		//Cara izquierda
		glNormal3f(-1.0,0.0,0.0);
		//glColor3f(0.0,1.0,0.0);
		glVertex3f(height/2,height/2,height/2);
		glVertex3f(height/2,-height/2,height/2);
		glVertex3f(height/2,-height/2,-height/2);
		glVertex3f(height/2,height/2,-height/2);
		//Cara trasera
		glNormal3f(0.0,0.0,-1.0);
		//glColor3f(0.0,0.0,1.0);
		glVertex3f(height/2,-height/2,-height/2);
		glVertex3f(height/2,height/2,-height/2);
		glVertex3f(-height/2,height/2,-height/2);
		glVertex3f(-height/2,-height/2,-height/2);
		//Cara derecha
		glNormal3f(1.0,0.0,0.0);
		//glColor3f(1.0,1.0,0.0);
		glVertex3f(-height/2,height/2,-height/2);
		glVertex3f(-height/2,-height/2,-height/2);
		glVertex3f(-height/2,-height/2,height/2);
		glVertex3f(-height/2,height/2,height/2);
		//Cara superior
		glNormal3f(0.0,1.0,0.0);
		//glColor3f(1.0,0.0,1.0);
		glVertex3f(height/2,height/2,height/2);
		glVertex3f(height/2,height/2,-height/2);
		glVertex3f(-height/2,height/2,-height/2);
		glVertex3f(-height/2,height/2,height/2);
		//Cara inferior
		glNormal3f(0.0,-1.0,0.0);
		//glColor3f(0.0,1.0,1.0);
		glVertex3f(-height/2,-height/2,height/2);
		glVertex3f(height/2,-height/2,height/2);
		glVertex3f(height/2,-height/2,-height/2);
		glVertex3f(-height/2,-height/2,-height/2);
	glEnd();
}

void init(){
	glClearColor(0.0,0.0,0.0,1.0);//r,g,b,alpha //Este sera el color del fondo, es decir el color al que se reinicializara el buffer de color
	glMatrixMode(GL_PROJECTION);//Indica que representar, le decimos que vamos a dar los parametros de la proyeccion
	glLoadIdentity();//Resetea la matriz de proyeccion a la identidad
	gluPerspective(45,640.0/480.0,1.0,500.0);//angulo, ancho/alto, distancia al plano de representacion mas cercano, distanca al plano de representacion mas lejano //Tambien se puede usar glOrtho de esta forma multiplicamos la matriz actual por una matriz ortografica, que simboliza una proyeccion paralela el (efecto que se obtendria no seria realista para imagenes 3d)
	glMatrixMode(GL_MODELVIEW);//Una vez terminamos de dar parametros volvemos al modo de vista del modelo	
	glShadeModel(GL_SMOOTH);//Esto nos permite decir como queremos que varien los colores, por defecto si un objeto tiene varios colores el color cambia de forma gradual, si queremos que el objeto sea del ultimo color que dimos hay q usar la opcin GL_FLAT
	//GL_SMOOTH es la opcion por defecto
	glEnable(GL_DEPTH_TEST);//Esta funcion permite habilitar varias opciones de openGL, esta opcion asigna un array a cada pixel de la pantalla para manejar la profundidad
	glEnable(GL_LIGHTING);//Habilita la iluminación
	glEnable(GL_LIGHT0);//Habilitamos un foco de luz
	//glEnable(GL_COLOR_MATERIAL);//Si habilitamos esto se crea un material de forma automatica con el color de la superficie
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
	DrawCube(1.0);
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
