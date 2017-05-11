#include "heightmap.h"

void loadHeightmap(std::vector<std::vector<float> >& heights, const char* name){
	
	SDL_Surface* img=SDL_LoadBMP(name);//Cargamos una imagen
	
	if(!img){//Si no hay imagen damos un mensaje de error
		std::cout<<"Error: image not found"<<std::endl;
		return;
	}
	std::vector<float> tmp;//Creamos una variable temporal
	heights.clear();//Borramos el vector que recibimos para almacenar valores
	
	for(int i=0;i<img->h;i++){//Recorremos los pixeles de la imagen
		
		tmp.clear();
		for(int j=0;j<img->w;j++){
			Uint32 pixel=((Uint32*)img->pixels)[i*img->pitch/4 + j];//Almacenamos la información del pixel actual
			Uint8 r,g,b;
			SDL_GetRGB(pixel,img->format,&r,&g,&b);
			tmp.push_back((float)r/255.0);//Esto va de 0.0 a 1.0 //Podriamos generar un heightmap también con g y b
		}
		heights.push_back(tmp);
	}
	

}

void renderHeightmap(std::vector<std::vector<float> >heights,float size,float h){
	
	for(int i=0;i<heights.size()-1;i++){
		for(int j=0;j<heights[i].size()-1;j++){
			
			glColor3f(heights[i][j],heights[i][j],heights[i][j]);//Damos colores distintos segun la altura para poder identificar bien el heightmap
			
			glBegin(GL_TRIANGLE_STRIP);				
				glVertex3f(i*size,heights[i][j]*h,j*size);
				glVertex3f((i+1)*size,heights[i+1][j]*h,j*size);
				glVertex3f(i*size,heights[i][j+1]*h,(j+1)*size);
				glVertex3f((i+1)*size,heights[i+1][j+1]*h,(j+1)*size);
			glEnd();
		}
		
	}
	
}
