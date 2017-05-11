#include "objLoader.h"

coordinate::coordinate(float a,float b,float c){
	x=a;
	y=b;
	z=c;
};

face::face(int facen,int f1,int f2,int f3,int t1,int t2,int t3,int m){
	facenum=facen;
	faces[0]=f1;
	faces[1]=f2;
	faces[2]=f3;
	texcoor[0]=t1;
	texcoor[1]=t2;
	texcoor[2]=t3;
	mat=m;
	four=false;
	
};

face::face(int facen,int f1,int f2,int f3,int f4,int t1,int t2,int t3,int t4,int m){
	facenum=facen;
	faces[0]=f1;
	faces[1]=f2;
	faces[2]=f3;
	faces[3]=f4;
	texcoor[0]=t1;
	texcoor[1]=t2;
	texcoor[2]=t3;
	texcoor[3]=t4;
	mat=m;
	four=true;//Si la cara esta formada por cuadrados este es el constructor que se emplea y entoces ponemos four a true
	
};

material::material(const char* na,float al,float nS,float nI,float *d,float *a,float *s,float i,int tex){//Simplemente asignamos cada cosa a su variable
	name=na;
	alpha=al;
	ns=nS;
	ni=nI;
	
	dif[0]=d[0];
	dif[1]=d[1];
	dif[2]=d[2];
	
	amb[0]=a[0];
	amb[1]=a[1];
	amb[2]=a[2];
	
	spec[0]=s[0];
	spec[1]=s[1];
	spec[2]=s[2];
	
	illum=i;
	texture=tex;
};

texcoord::texcoord(float a,float b){
	u=a;
	v=b;
};

void objLoader::clear(){
	for(int i;i<coord.size();i++)delete coord[i];//Cuando terminamos borramos el vector coord
	for(int i;i<faces.size();i++)delete faces[i];//Tambien borramos el vector faces
	for(int i;i<normals.size();i++)delete normals[i];//Tambien borramos el vector normals
	for(int i;i<vertex.size();i++)delete vertex[i];//Tambien borramos el vector vertex
	for(int i;i<materials.size();i++)delete materials[i];//Tambien borramos el vector materials
	for(int i;i<texturecoord.size();i++)delete texturecoord[i];//Tambien borramos el vector texturecoord
	for(int i;i<vertexnormals.size();i++)delete vertexnormals[i];//Tambien borramos el vector vertexnormals
	//En los ejemplos anteriores faltaba llamar a la funcion clear de los vectores para eliminar los punteros
	coord.clear();
	faces.clear();
	normals.clear();
	vertex.clear();
	materials.clear();
	texturecoord.clear();
	vertexnormals.clear();
}

int objLoader::loadObject(std::string filename){
	std::ifstream in(filename.c_str());
	if(!in.is_open()){
		std::cout<<"File not found."<<std::endl;
		return -1;
	}
	
	std::string path=filename.substr(0,(filename.find_last_of('/') != std::string::npos ? filename.find_last_of('/')+1 : 0));//Almacenamos la dirección del archivo
	
	char c[256];//Creamos un buffer en el que almacenar lo que leemos
	int curmat=0;//En esta variable almacenaremos el identificador del material
	while(!in.eof()){
		in.getline(c,256);
		coord.push_back(new std::string(c));
	}
	
	for(int i=0;i<coord.size();i++){
		if((*coord[i])[0]=='#'){//Si el primer simbolo de la linea es # la ignoramos ya que es un comentario
		}else if((*coord[i])[0]=='v'&&(*coord[i])[1]==' '){
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);
			vertex.push_back(new coordinate(tmpx,tmpy,tmpz));
		}else if((*coord[i])[0]=='v'&&(*coord[i])[1]=='n'){
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
			normals.push_back(new coordinate(tmpx,tmpy,tmpz));
			isnormals=true;
		}else if((*coord[i])[0]=='f'){
			int a,b,c,d,e;
			if(count(coord[i]->begin(),coord[i]->end(),' ')==4){//Count nos devuelve el numero de elementos en rango
				if(coord[i]->find("//")!=std::string::npos){//Find nos dira si tenemos algun // en el archivo y asi podremos decidir en que caso de cara&material estamos
					sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);//Almacenamos las variables
					faces.push_back(new face(b,a,c,d,e,0,0,0,0,curmat));
				}else if(coord[i]->find('/')!=std::string::npos){//Aqui buscamos un / en lugar de dos
					int t[4];//Cuidado aqui
					sscanf(coord[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",&a,&t[0],&b,&c,&t[1],&b,&d,&t[2],&b,&e,&t[3],&b);//Almacenamos las variables
					faces.push_back(new face(b,a,c,d,e,t[0],t[1],t[2],t[3],curmat));
				}else{
					sscanf(coord[i]->c_str(),"f %d %d %d %d",&a,&b,&c,&d);//Almacenamos las variables
					faces.push_back(new face(-1,a,b,c,d,0,0,0,0,curmat));
				}
			}else{//En caso de que la cara este formada por triangulos tendra un vertice menos, en ese caso llamamos al otro constructor
				if(coord[i]->find("//")!=std::string::npos){//Find nos dira si tenemos algun // en el archivo y asi podremos decidir en que caso de cara&material estamos
					sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);//Almacenamos las variables
					faces.push_back(new face(b,a,c,d,0,0,0,curmat));
				}else if(coord[i]->find('/')!=std::string::npos){//Aqui buscamos un / en lugar de dos
					int t[3];//Cuidado aqui
					sscanf(coord[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&a,&t[0],&b,&c,&t[1],&b,&d,&t[2],&b);//Almacenamos las variables
					faces.push_back(new face(b,a,c,d,t[0],t[1],t[2],curmat));
				}else{
					sscanf(coord[i]->c_str(),"f %d %d %d",&a,&b,&c);//Almacenamos las variables
					faces.push_back(new face(-1,a,b,c,0,0,0,curmat));
				}
			}
		}
	else if((*coord[i])[0]=='u'&&(*coord[i])[1]=='s'&&(*coord[i])[2]=='e'&&(*coord[i])[3]=='m'&&(*coord[i])[4]=='t'&&(*coord[i])[5]=='l'){
		char tmp[200];//Cuidado arriba con un {
		sscanf(coord[i]->c_str(),"usemtl %s",tmp);//Leemos el nombre y lo almacenamos en tmp
		for(int i=0;i<materials.size();i++){
			if(strcmp(materials[i]->name.c_str(),tmp)==0){//Comparamos los dos strings
				curmat=i;
				break;//Esto es para terminar la busqueda y no malgastar procesador
			}
		}
	}else if((*coord[i])[0]=='m'&&(*coord[i])[1]=='t'&&(*coord[i])[2]=='l'&&(*coord[i])[3]=='l'&&(*coord[i])[4]=='i'&&(*coord[i])[5]=='b'){
		char filen[200];
		sscanf(coord[i]->c_str(),"mtllib %s",filen);//Guardamos el nombre del archivo
		std::string filen2=path+filen;
		std::ifstream mtlin(filen2.c_str());//Abrimos el archivo
		
		if(!mtlin.is_open()){
			 std::cout<<"Material file not found."<<std::endl;
			clear();
			return 1;//Podriamos no cerrar y simplemente continuar sin material
		}
		ismaterial=true;//Si el archivo se abre correctamente decmos que tenemos material para el objeto
		std::vector<std::string> tmp;
		char c[200];//string para leer
		while(!mtlin.eof()){//Mientras no llegemos al final del archivo continuamos leyendo
			mtlin.getline(c,200);//Leemos una linea completa o 200 char como max
			tmp.push_back(c);//Vamos almacenando el archivo completo en tmp
		}
		char name[200];
		char filename[200];
		float amb[3],dif[3],spec[3],alpha,ni,ns;//estas variables son para almacenar parametros del matrial
		float illum;//Esta es para la iluminacion o algo asi
		unsigned int texture;//identificador de la textura?
		bool ismat=false;//Creamos la variable de material y de mometo la ponemos a false
		strcpy(filename,"\0");//Almacenamos en filename el caracter null
		for(int i=0;i<tmp.size();i++){//Recorremos tmp
			if(tmp[i][0]=='#'){//Si la linea en la que estamos es un comentario ni la leemos
			}else{
				if(tmp[i][0]=='n'&&tmp[i][1]=='e'&&tmp[i][2]=='w'&&tmp[i][3]=='m'&&tmp[i][4]=='t'&&tmp[i][5]=='l'){//Si tenemos un nuevo material
					if(ismat){//si ya hay material
						if(strcmp(filename,"\0")!=0){//Si filename no esta vacio
							materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,texture));
							strcpy(filename,"\0");//Volvemos a poner un null en filename
						}else{
							materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,-1));
						}
					}
					ismat=false;
					sscanf(tmp[i].c_str(),"newmtl %s",name);
				}else if(tmp[i][0]=='N'&&tmp[i][1]=='s'){
					sscanf(tmp[i].c_str(),"Ns %f",&ns);//Leemos el parametro ns
					ismat=true;//Ya tenemos algo en el material, por lo que lo ponemos a true
				}else if(tmp[i][0]=='K'&&tmp[i][1]=='a'){
					sscanf(tmp[i].c_str(),"Ka %f %f %f",&amb[0],&amb[1],&amb[2]);//Leemos el parametro ka
					ismat=true;//Ya tenemos algo en el material, por lo que lo ponemos a true
				}else if(tmp[i][0]=='K'&&tmp[i][1]=='d'){
					sscanf(tmp[i].c_str(),"Kd %f %f %f",&dif[0],&dif[1],&dif[2]);//Leemos el parametro kd
					ismat=true;//Ya tenemos algo en el material, por lo que lo ponemos a true
				}else if(tmp[i][0]=='K'&&tmp[i][1]=='s'){
					sscanf(tmp[i].c_str(),"Ks %f %f %f",&spec[0],&spec[1],&spec[2]);//Leemos el parametro ks
					ismat=true;//Ya tenemos algo en el material, por lo que lo ponemos a true
				}else if(tmp[i][0]=='N'&&tmp[i][1]=='i'){
					sscanf(tmp[i].c_str(),"Ni %f",&ni);//Leemos el parametro ni
					ismat=true;//Ya tenemos algo en el material, por lo que lo ponemos a true
				}else if(tmp[i][0]=='d'&&tmp[i][1]==' '){
					sscanf(tmp[i].c_str(),"d %f",&alpha);//Leemos el parametro alpha
					ismat=true;//Ya tenemos algo en el material, por lo que lo ponemos a true
				}else if(tmp[i][0]=='i'&&tmp[i][1]=='l'&&tmp[i][2]=='l'&&tmp[i][3]=='u'&&tmp[i][4]=='m'){
					sscanf(tmp[i].c_str(),"illum %f",&illum);//Leemos el parametro illum
					ismat=true;//Ya tenemos algo en el material, por lo que lo ponemos a true
				}else if(tmp[i][0]=='m'&&tmp[i][1]=='a'&&tmp[i][2]=='p'&&tmp[i][3]=='_'&&tmp[i][4]=='K'&&tmp[i][5]=='d'){
					sscanf(tmp[i].c_str(),"map_Kd %s",filename);
					texture=loadTexture(filename);
					ismat=true;
				}
			}
		}
		//Ponemos esto aqui para que no se salte el ultimo material
		if(ismat){//si ya hay material
			if(strcmp(filename,"\0")!=0){//Si filename no esta vacio
				materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,texture));
			}else{
				materials.push_back(new material(name,alpha,ns,ni,dif,amb,spec,illum,-1));
			}
		}
		
		
	}else if((*coord[i])[0]=='v'&&(*coord[i])[1]=='t'){//si tenemos un vertice de la textura almacenamos sus coordenadas
		float u,v;
		sscanf(coord[i]->c_str(),"vt %f %f",&u,&v);
		texturecoord.push_back(new texcoord(u,1-v));//almacenamos las coordenadas en el vector, el 1-v es por unc ambio del origen de coordenadas
		istexture=true;
	}
	
	if(materials.size()==0){//comprobamos el tamaño del vector de materiales y ponemos ismaterial a false si es 0
		ismaterial=false;
	}else{
		ismaterial=true;
	}
}
	
	//Aqui dibujamos
	
	if(smoothnormals)smooth();//Si tenemos la opcion de suavizar activada llamamos a la funcion que suaviza los cambios de las caras
	
	int num;
	num=glGenLists(1);//Cada vez que llamamos a GenLists el numero que obtenemos se incrementa en el numero de unidades que indicamos
	int last=-1;//Esta variable almacenara el ultimo material que usamos
	glNewList(num,GL_COMPILE);//Creamos una nueva lista y de momento solo compilamos
	
	
	
		for(int i=0;i<faces.size();i++){
			//Antes de dibujar las caras nos ocupamos de materiales y texturas
			if(last!=faces[i]->mat && ismaterial){//Solo cargamos un material si el material ha cambiado y hay material que cargar
				float diffuse[]={materials[faces[i]->mat]->dif[0],materials[faces[i]->mat]->dif[1],materials[faces[i]->mat]->dif[2],1.0};//Creamos un vector con los parametros de reflexion difusa, el ultimo parametro es la transparencia, de momento lo dejamos a 1.0 pero lo podemos poner a alpha mas adelante
				float ambient[]={materials[faces[i]->mat]->amb[0],materials[faces[i]->mat]->amb[1],materials[faces[i]->mat]->amb[2],1.0};//Lo mismo con la reflexion de luz ambiental y la reflexion especular
				float specular[]={materials[faces[i]->mat]->spec[0],materials[faces[i]->mat]->spec[1],materials[faces[i]->mat]->spec[2],1.0};
				glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);//Establecemos los valores de las propiedades anteriores
				glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
				glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
				glMaterialf(GL_FRONT,GL_SHININESS,materials[faces[i]->mat]->ns);//Establecemos el valor del brillo
				last=faces[i]->mat;//Damos a last el valor del material que hemos usado
				if(materials[faces[i]->mat]->texture==-1){//Si no hay textura deshabilitamos las texturas
					glDisable(GL_TEXTURE_2D);
				}else{//Si no, las habilitamos y establecemos la textura actual
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D,materials[faces[i]->mat]->texture);
				}
			}
			
			
			if(faces[i]->four){//Si la cara esta formada por un cuadrado/rectangulo
			
			
				glBegin(GL_QUADS);
				

				
					if(isnormals)glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);//Creamos un vector normal a la cara
					
					if(istexture && materials[faces[i]->mat]->texture!=-1){//Comprobamos que hay textura o material y aplicamos los vertices
						glTexCoord2f(texturecoord[faces[i]->texcoor[0]-1]->u,texturecoord[faces[i]->texcoor[0]-1]->v);
					}
					
					if(smoothnormals){
						glNormal3f(vertexnormals[faces[i]->faces[0]-1]->x,vertexnormals[faces[i]->faces[0]-1]->y,vertexnormals[faces[i]->faces[0]-1]->z);
					}
					
					glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);//Creamos vertices para las caras
					
					if(istexture && materials[faces[i]->mat]->texture!=-1){//Comprobamos que hay textura o material y aplicamos los vertices
						glTexCoord2f(texturecoord[faces[i]->texcoor[1]-1]->u,texturecoord[faces[i]->texcoor[1]-1]->v);
					}
						
					if(smoothnormals){
						glNormal3f(vertexnormals[faces[i]->faces[1]-1]->x,vertexnormals[faces[i]->faces[1]-1]->y,vertexnormals[faces[i]->faces[1]-1]->z);
					}
										
					glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
					
					if(istexture && materials[faces[i]->mat]->texture!=-1){//Comprobamos que hay textura o material y aplicamos los vertices
						glTexCoord2f(texturecoord[faces[i]->texcoor[2]-1]->u,texturecoord[faces[i]->texcoor[2]-1]->v);
					}
					
					if(smoothnormals){
						glNormal3f(vertexnormals[faces[i]->faces[2]-1]->x,vertexnormals[faces[i]->faces[2]-1]->y,vertexnormals[faces[i]->faces[2]-1]->z);
					}
					
					glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
					
					if(istexture && materials[faces[i]->mat]->texture!=-1){//Comprobamos que hay textura o material y aplicamos los vertices
						glTexCoord2f(texturecoord[faces[i]->texcoor[3]-1]->u,texturecoord[faces[i]->texcoor[3]-1]->v);
					}
					
					if(smoothnormals){
						glNormal3f(vertexnormals[faces[i]->faces[3]-1]->x,vertexnormals[faces[i]->faces[3]-1]->y,vertexnormals[faces[i]->faces[3]-1]->z);
					}
					
					glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
				glEnd();
			}else{//En caso de que sea un triangulo
				glBegin(GL_TRIANGLES);
							
					if(isnormals)glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);//Creamos un vector normal a la cara
					
					
					if(istexture && materials[faces[i]->mat]->texture!=-1){//Comprobamos que hay textura o material y aplicamos los vertices
						glTexCoord2f(texturecoord[faces[i]->texcoor[0]-1]->u,texturecoord[faces[i]->texcoor[0]-1]->v);
					}
					
					if(smoothnormals){
						glNormal3f(vertexnormals[faces[i]->faces[0]-1]->x,vertexnormals[faces[i]->faces[0]-1]->y,vertexnormals[faces[i]->faces[0]-1]->z);
					}
					
					glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);//Creamos vertices para las caras
					
					if(istexture && materials[faces[i]->mat]->texture!=-1){//Comprobamos que hay textura o material y aplicamos los vertices
						glTexCoord2f(texturecoord[faces[i]->texcoor[1]-1]->u,texturecoord[faces[i]->texcoor[1]-1]->v);
					}
					
					if(smoothnormals){
						glNormal3f(vertexnormals[faces[i]->faces[1]-1]->x,vertexnormals[faces[i]->faces[1]-1]->y,vertexnormals[faces[i]->faces[1]-1]->z);
					}
					
					glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
					
					if(istexture && materials[faces[i]->mat]->texture!=-1){//Comprobamos que hay textura o material y aplicamos los vertices
						glTexCoord2f(texturecoord[faces[i]->texcoor[2]-1]->u,texturecoord[faces[i]->texcoor[2]-1]->v);
					}
					
					if(smoothnormals){
						glNormal3f(vertexnormals[faces[i]->faces[2]-1]->x,vertexnormals[faces[i]->faces[2]-1]->y,vertexnormals[faces[i]->faces[2]-1]->z);
					}
					
					glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
				glEnd();
			}
		}
		
		glEndList();
		clear();
		lists.push_back(num);//Metemos num en la lista
		
		
		return num;//Devolvemos un identificador para la lista
	
}



unsigned int objLoader::loadTexture(const char* filename, bool generate){
	
	
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
	texture.push_back(id);//Metemos el identificador en el vector de texturas
	
	return id;//Devolvemos el id de la textura
	
}

objLoader::~objLoader(){
	for(std::vector<unsigned int>::const_iterator it=texture.begin();it!=texture.end();it++){//Creamos un iterador para la lista y mientras no llequemos al final de esta continuamos con el bucle
		
		glDeleteTextures(1,&(*it));//Damos el numero de elementos a eliminar y la direccion de memoria
	}
	for(std::vector<unsigned int>::const_iterator it=lists.begin();it!=lists.end();it++){//Creamos un iterador para la lista y mientras no llequemos al final de esta continuamos con el bucle
		
		glDeleteLists(*it,1);//Damos el elemento a borrar y el numero de elementos
	}
}

objLoader::objLoader(){
	ismaterial=false;
    isnormals=false;
    istexture=false;
    smoothnormals=true;
}

void objLoader::smooth(){
	
	for(int i=1;i<vertex.size()+1;i++){//Vamos de 1 a size+1 pq en vertex no hay 1er vector
		float vecX=0.0,vecY=0.0,vecZ=0.0;
		int num=0;
		for(int j=0;j<faces.size();j++){
			if(faces[j]->faces[0]==i || faces[j]->faces[1]==i || faces[j]->faces[2]==i || faces[j]->faces[3]==i){//Comprobamos los vectores de cada cara y sumamos los vectores que pertenezcan al mismo vertice
				vecX+=normals[faces[j]->facenum-1]->x;
				vecY+=normals[faces[j]->facenum-1]->y;
				vecZ+=normals[faces[j]->facenum-1]->z;
				num++;//Aumentamos el numero de vectores para hacer luego la media
			}
		}
		if(num){//Comprobamos que num no es 0
			
			vecX/=num;
			vecY/=num;
			vecZ/=num;
			//Dividimos entre num para hacer la media
		}
		float mod=sqrt(vecX*vecX+vecY*vecY+vecZ*vecZ);//Calculamos el modulo del vector para normalizar
		if(mod){//De nuevo nos aseguramos de no dividir entre 0
			
			vecX/=mod; //Si se desactiva el objeto se vuelve mas oscuro, pero al eliminar la raiz cuadrada consume menos
			vecY/=mod;
			vecZ/=mod;
			//Normalizamos el vector para que sea un vector normal
		}
		vertexnormals.push_back(new coordinate(vecX,vecY,vecZ));//Almacenamos el vector
	}
	
}
