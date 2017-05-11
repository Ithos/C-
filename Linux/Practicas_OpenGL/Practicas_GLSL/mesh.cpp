#include "mesh.h"

meshClass::meshClass(std::vector<vertexData>* vd,std::vector<unsigned int>* id,std::vector<textureData>* td){
	data=*vd;//Almacenamos los parámetros que recibimos
	indexes=*id;
	if(td!=NULL)textures=*td;
	
	glGenBuffers(1,&VBO);//Generamos los buffers tanto para vertices como para indices
	glBindBuffer(GL_ARRAY_BUFFER,VBO);//Hacemos que el buffer almacene arrays
	glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(vertexData),&data[0],GL_STATIC_DRAW);
	
	glGenBuffers(1,&IND);//Lo mismo para el buffer de indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);//En este caso solo almacenamos un elemento 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexes.size()*sizeof(unsigned int),&indexes[0],GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);//Borramos los datos de los buffers antes de empezar a usarlos
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

meshClass::~meshClass(){
	glDeleteBuffers(1,&VBO);//Borramos los buffers
	glDeleteBuffers(1,&IND);
}

void meshClass::draw(unsigned int programId){
	int vertex=glGetAttribLocation(programId,"vertex");//Leemos variables del shader
	int normal=glGetAttribLocation(programId,"normal");
	int tangent=glGetAttribLocation(programId,"tangent");
	int color=glGetAttribLocation(programId,"color");
	int UV=glGetAttribLocation(programId,"UV");
	
	//Recorremos las texturas y unimos las texturas a los vertices
	std::string str="texture";
	for(int i=0;i<textures.size();i++){
		glActiveTexture(GL_TEXTURE0 + i);//Seleccionamos la textura activa, cada vertice debería poder almacenar 4 como minimo
		glBindTexture(GL_TEXTURE_2D,textures[i].id);
		glUniform1i(glGetUniformLocation(programId,(str + (char)(i + '0')).c_str()),i);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);
	
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);//Aqui decimos de donde leer los datos de la posición y cada cuanto leerlos (ir a la documentación para más detalles) si da problemas hay que hacer los atributos de la clase vector3d públicos
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));//Lo mismo de antes para decir dónde se almacenan los vectores normales //El cast a void* es necesario para que el programa compile aunque sea un horror
	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));//Estos valores se obtienen del struct vertexData o tienen la misma estructura o algo así
	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));//en este caso solo tenemos 2 componentes
	
	glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_INT,0);//Dibujamos lo que hemos almacenado en los buffers
	
	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);//Borramos los datos de los buffers 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}
