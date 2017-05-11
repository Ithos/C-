#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

void init(){
	glClearColor(0.0,0.0,0.0,1.0);//r,g,b,alpha //Este sera el color del fondo
	glMatrixMode(GL_PROJECTION);//Indica que representar, le decimos que vamos a dar los parametros de la proyeccion
	glLoadIdentity();//Resetea la matriz de proyeccion a la identidad
	gluPerspective(45,640.0/480.0,1.0,500.0);//angulo, ancho/alto, distancia al plano de representacion mas cercano, distanca al plano de representacion mas lejano
	glMatrixMode(GL_MODELVIEW);//Una vez terminamos de dar parametros volvemos al modo de vista del modelo	
	glShadeModel(GL_SMOOTH);//Esto nos permite decir como queremos que varien los colores, por defecto si un objeto tiene varios colores el color cambia de forma gradual, si queremos que el objeto sea del ultimo color que dimos hay q usar la opcin GL_FLAT
	//GL_SMOOTH es la opcion por defecto
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);//Le decimos que buffer usar
	glBegin(GL_TRIANGLES);
		glColor3f(1.0,0.0,0.0);//damos el color como 3 float en rgb, esta funcion va como una maquina de estados, es decir se queda en el ultimo color que diste
		glVertex3f(0.0,2.0,-7.0);//3f es por los 3 parametros float, damos x,y,z //z es profundidad //Damos las coordenadas de los vertices
		glColor3f(0.0,1.0,0.0);//si empleamos varios colores en un objeto normalmente el color cambia de forma gradual de uno a otro
		glVertex3f(-2.0,-2.0,-7.0);//Nota: El origen de coordenadas es el centro de la pantalla
		glColor3f(0.0,0.0,1.0);//Estamos usando un color distinto en cada vertice para ver como cambian los colores
		glVertex3f(2.0,-2.0,-7.0);
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
		
		if(1000/30>(SDL_GetTicks()-start)){
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
		}
	}
	SDL_Quit();
	return 0;
}
