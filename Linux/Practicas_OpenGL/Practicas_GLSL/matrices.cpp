#include "matrices.h"

matrices::matrices(){
	modelMatrix.push_back(glm::mat4(1.0));//Cargamos la matriz identidad por defecto
	viewMatrix.push_back(glm::mat4(1.0));
	projectionMatrix.push_back(glm::mat4(1.0));
	currentMatrix=MODEL_MATRIX;
	matricesReady=true;
	modelViewMatrix=glm::mat4(1.0);
	modelViewProjectionMatrix=glm::mat4(1.0);
	normalMatrix=glm::mat3(1.0);
}

void matrices::loadIdentity(){
	if(currentMatrix==MODEL_MATRIX || currentMatrix==VIEW_MATRIX){
		modelMatrix[modelMatrix.size()-1]=glm::mat4(1.0);
		viewMatrix[viewMatrix.size()-1]=glm::mat4(1.0);
	}else{
		projectionMatrix[projectionMatrix.size()-1]=glm::mat4(1.0);
	}
	matricesReady=false;
}

void matrices::matrixMode(int i){
	if(i==MODEL_MATRIX){
		currentMatrix=MODEL_MATRIX;
	}else if(i==VIEW_MATRIX){
		currentMatrix=VIEW_MATRIX;
	}else if(i==PROJECTION_MATRIX){
		currentMatrix=PROJECTION_MATRIX;
	}
}

void matrices::translate(float x,float y,float z){
	//Múltiplicamos la matriz correspondiente por la matriz de la transformación
	if(currentMatrix==MODEL_MATRIX){
		modelMatrix[modelMatrix.size()-1]*=glm::translate(glm::mat4(1.0f),glm::vec3(x,y,z));
	}else if(currentMatrix==VIEW_MATRIX){//Esto se corresponde con la camara por lo que la transformación es distinta
		viewMatrix[viewMatrix.size()-1]*=glm::translate(glm::mat4(1.0f),glm::vec3(-x,-y,-z));
	}
	
	matricesReady=false;
	
}

void matrices::scale(float x,float y,float z){
	//Múltiplicamos la matriz correspondiente por la matriz de la transformación
	if(currentMatrix==MODEL_MATRIX){
		modelMatrix[modelMatrix.size()-1]*=glm::scale(glm::mat4(1.0f),glm::vec3(x,y,z));
	}else if(currentMatrix==VIEW_MATRIX){//Esto se corresponde con la camara por lo que la transformación es distinta
		viewMatrix[viewMatrix.size()-1]*=glm::scale(glm::mat4(1.0f),glm::vec3(-x,-y,-z));
	}
	
	matricesReady=false;
	
}

void matrices::scale(float x){
	//Múltiplicamos la matriz correspondiente por la matriz de la transformación
	if(currentMatrix==MODEL_MATRIX){
		modelMatrix[modelMatrix.size()-1]*=glm::scale(glm::mat4(1.0f),glm::vec3(x,x,x));
	}else if(currentMatrix==VIEW_MATRIX){//Esto se corresponde con la camara por lo que la transformación es distinta
		viewMatrix[viewMatrix.size()-1]*=glm::scale(glm::mat4(1.0f),glm::vec3(-x,-x,-x));
	}
	
	matricesReady=false;
	
}

void matrices::rotateX(float angle){
	//Múltiplicamos la matriz correspondiente por la matriz de la transformación
	if(currentMatrix==MODEL_MATRIX){
		modelMatrix[modelMatrix.size()-1]*=glm::rotate(glm::mat4(1.0f),angle,glm::vec3(1.0f,0.0f,0.0f));
	}else if(currentMatrix==VIEW_MATRIX){//Esto se corresponde con la camara por lo que la transformación es distinta
		viewMatrix[viewMatrix.size()-1]*=glm::rotate(glm::mat4(1.0f),-angle,glm::vec3(1.0f,0.0f,0.0f));
	}
	
	matricesReady=false;
	
}

void matrices::rotateY(float angle){
	//Múltiplicamos la matriz correspondiente por la matriz de la transformación
	if(currentMatrix==MODEL_MATRIX){
		modelMatrix[modelMatrix.size()-1]*=glm::rotate(glm::mat4(1.0f),angle,glm::vec3(0.0f,1.0f,0.0f));
	}else if(currentMatrix==VIEW_MATRIX){//Esto se corresponde con la camara por lo que la transformación es distinta
		viewMatrix[viewMatrix.size()-1]*=glm::rotate(glm::mat4(1.0f),-angle,glm::vec3(0.0f,1.0f,0.0f));
	}
	
	matricesReady=false;
	
}

void matrices::rotateZ(float angle){
	//Múltiplicamos la matriz correspondiente por la matriz de la transformación
	if(currentMatrix==MODEL_MATRIX){
		modelMatrix[modelMatrix.size()-1]*=glm::rotate(glm::mat4(1.0f),angle,glm::vec3(0.0f,0.0f,1.0f));
	}else if(currentMatrix==VIEW_MATRIX){//Esto se corresponde con la camara por lo que la transformación es distinta
		viewMatrix[viewMatrix.size()-1]*=glm::rotate(-glm::mat4(1.0f),-angle,glm::vec3(0.0f,0.0f,1.0f));
	}
	
	matricesReady=false;
	
}

void matrices::perspective(float angle,float aRatio,float near,float far){
	projectionMatrix[projectionMatrix.size()-1]=glm::perspective(angle,aRatio,near,far);
	
	matricesReady=false;
	
}

void matrices::ortho(float left,float right,float bottom,float top,float near,float far){
	projectionMatrix[projectionMatrix.size()-1]=glm::ortho(left,right,bottom,top,near,far);
	
	matricesReady=false;
	
}

void matrices::pushMatrix(){
	//Guardamos la matriz actual en la pila
	glm::mat4 matrix;
	if(currentMatrix==MODEL_MATRIX){
		matrix=modelMatrix[modelMatrix.size()-1];
		modelMatrix.push_back(matrix);
	}else if(currentMatrix==VIEW_MATRIX){
		viewMatrix[viewMatrix.size()-1];
		viewMatrix.push_back(matrix);
	}else{
		matrix=projectionMatrix[projectionMatrix.size()-1];
		projectionMatrix.push_back(matrix);
	}
	
	matricesReady=false;
	
}

void matrices::popMatrix(){
	//expulsamos de la pila la matriz actual si tenemos mas de 1
	if(currentMatrix==MODEL_MATRIX){
		if(modelMatrix.size()>1)modelMatrix.pop_back();
	}else if(currentMatrix==VIEW_MATRIX){
		if(viewMatrix.size()>1)viewMatrix.pop_back();
	}else{
		if(projectionMatrix.size()>1)projectionMatrix.pop_back();
	}
	
	matricesReady=false;
	
}

glm::mat4& matrices::getModelMatrix(){
	return modelMatrix[modelMatrix.size()-1];
}

glm::mat4& matrices::getModelViewMatrix(){
	if(!matricesReady) modelViewMatrix=viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
	return modelViewMatrix;
}

glm::mat4& matrices::getModelViewProjectionMatrix(){
	if(!matricesReady) modelViewProjectionMatrix=projectionMatrix[projectionMatrix.size()-1]*viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
	return modelViewProjectionMatrix;
}

glm::mat3& matrices::getNormalMatrix(){
	//El método que vamos a usar para calcular los vectores normales solo vale si se escala de forma uniforme, en caso de que se quiera emplear escalado no uniforme hay que emplear la inversa de la traspuesta de la matriz de traslación esto es debido a que no debemos trasladar los vectores normales
	if(!matricesReady) normalMatrix=glm::mat3(getModelViewMatrix());
	return normalMatrix;
}

glm::mat4& matrices::getViewMatrix(){
	return viewMatrix[viewMatrix.size()-1];
}
glm::mat4& matrices::getProjectionMatrix(){
	return projectionMatrix[projectionMatrix.size()-1];
}

void matrices::updateMatrices(unsigned int programId){
	if(!matricesReady){
		modelViewMatrix=viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		modelViewProjectionMatrix=projectionMatrix[projectionMatrix.size()-1]*viewMatrix[viewMatrix.size()-1]*modelMatrix[modelMatrix.size()-1];
		normalMatrix=glm::mat3(getModelViewMatrix());//Aqui habría que cambiar como se calcula la normalMatrix
		
		matricesReady=true;
	}
	
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelMatrix"),1,GL_FALSE,&modelMatrix[modelMatrix.size()-1][0][0]);//Mandamos modelMatrix al shader
	glUniformMatrix4fv(glGetUniformLocation(programId,"viewMatrix"),1,GL_FALSE,&viewMatrix[viewMatrix.size()-1][0][0]);//Mandamos viewMatrix al shader
	glUniformMatrix4fv(glGetUniformLocation(programId,"projectionMatrix"),1,GL_FALSE,&projectionMatrix[projectionMatrix.size()-1][0][0]);//Mandamos projectionMatrix al shader
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewMatrix"),1,GL_FALSE,&modelViewMatrix[0][0]);//Mandamos modelViewMatrix al shader
	glUniformMatrix4fv(glGetUniformLocation(programId,"modelViewProjectionMatrix"),1,GL_FALSE,&modelViewProjectionMatrix[0][0]);//Mandamos modelViewProjectionMatrix al shader
	glUniformMatrix3fv(glGetUniformLocation(programId,"normalMatrix"),1,GL_FALSE,&normalMatrix[0][0]);//Mandamos normalMatrix al shader
	
}
