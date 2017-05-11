#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

float angle=0.0;

void init(){
	glClearColor(0.0,0.0,0.0,1.0);//r,g,b,alpha //Este sera el color del fondo
	glMatrixMode(GL_PROJECTION);//Indica que representar, le decimos que vamos a dar los parametros de la proyeccion
	glLoadIdentity();//Resetea la matriz de proyeccion a la identidad
	gluPerspective(45,640.0/480.0,1.0,500.0);//angulo, ancho/alto, distancia al plano de representacion mas cercano, distanca al plano de representacion mas lejano //Tambien se puede usar glOrtho de esta forma multiplicamos la matriz actual por una matriz ortografica, que simboliza una proyeccion paralela el (efecto que se obtendria no seria realista para imagenes 3d)
	glMatrixMode(GL_MODELVIEW);//Una vez terminamos de dar parametros volvemos al modo de vista del modelo	
	glShadeModel(GL_SMOOTH);//Esto nos permite decir como queremos que varien los colores, por defecto si un objeto tiene varios colores el color cambia de forma gradual, si queremos que el objeto sea del ultimo color que dimos hay q usar la opcin GL_FLAT
	//GL_SMOOTH es la opcion por defecto
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);//Le decimos que buffer usar
	glLoadIdentity();//Con esto reseteamos la matriz a la identidad, de esta forma evitamos que la siguiente traslacion se aplique continuamente y conseguimos que se aplique solo una vez
	glTranslatef(0.0,0.0,-5.0);//Cada vez que lo llamemos multiplica la matriz actual por una de traslacion que indiquemos moviendo cosas en uno o varios ejes
	glRotatef(angle,0.0,0.0,1.0);//Mediante esta funcion rotamos en un eje //angulo, ejes(x,y,z)
	glScalef(1.0,0.5,1.0);//Multiplicamos el eje correspondiente por el numero indicado (x,y,z)
	glBegin(GL_TRIANGLES);//La traslacion de arriba se aplica a todas las imagenes a partir de su ejecucion
		glColor3f(1.0,0.0,0.0);//damos el color como 3 float en rgb, esta funcion va como una maquina de estados, es decir se queda en el ultimo color que diste
		glVertex3f(0.0,2.0,0.0);//3f es por los 3 parametros float, damos x,y,z //z es profundidad //Damos las coordenadas de los vertices
		glColor3f(0.0,1.0,0.0);//si empleamos varios colores en un objeto normalmente el color cambia de forma gradual de uno a otro
		glVertex3f(-2.0,-2.0,0.0);//Nota: El origen de coordenadas es el centro de la pantalla
		glColor3f(0.0,0.0,1.0);//Estamos usando un color distinto en cada vertice para ver como cambian los colores
		glVertex3f(2.0,-2.0,0.0);
	glEnd();
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
		
		if(1000/30>(SDL_GetTicks()-start)){
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
		}
	}
	SDL_Quit();
	return 0;
}
