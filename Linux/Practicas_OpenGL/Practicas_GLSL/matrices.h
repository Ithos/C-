#ifndef MATRICES_H
#define MATRICES_H
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLee.h"
//En esta clase sobreescribimos las matrices por defecto y varias funciones de OpenGl

enum matrixModes{
	MODEL_MATRIX=0,
	VIEW_MATRIX,
	PROJECTION_MATRIX
};

class matrices{
	std::vector<glm::mat4> modelMatrix;
	std::vector<glm::mat4> viewMatrix;
	//modelViewMatrix=viewMatrix*modelMatrix -> asi es como calculamos modelViewMatrix
	std::vector<glm::mat4> projectionMatrix;
	//modelViewProjectionMatrix=projectionMatrix*modelViewMatrix -> formula para calcular modelViewProjectionMatrix
	
	//esto es el equivalente a glMatrixMode
	int currentMatrix;
	
	//Vamos a mejorar el rendimiento precalculando las matrices solo cuando sea necesario
	bool matricesReady;//será true si no es necesario recalcular las matrices
	glm::mat4 modelViewMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat3 normalMatrix;
	
	public:
	matrices();
	
	void loadIdentity();//glLoadIdentity
	void matrixMode(int i);//glMatrixMode
	
	//Transformaciones
	void translate(float x,float y,float z);//glTranslate()
	void scale(float x,float y,float z);//glScalef(x,y,z)
	void scale(float x);//glScalef(x,x,x)
	void rotateX(float angle);//glRotatef(angle,1,0,0)
	void rotateY(float angle);//glRotatef(angle,0,1,0)
	void rotateZ(float angle);//glRotatef(angle,0,0,1)
	
	//Proyecciones
	void perspective(float angle,float aRatio,float near,float far);//gluPerspective
	void ortho(float left,float right,float bottom,float top,float near,float far);//glOrtho
	
	void pushMatrix();//glPushMatrix
	void popMatrix();//glPopMatrix
	
	glm::mat4& getModelMatrix();
	glm::mat4& getViewMatrix();
	glm::mat4& getProjectionMatrix();
	glm::mat4& getModelViewMatrix();
	glm::mat4& getModelViewProjectionMatrix();
	glm::mat3& getNormalMatrix();
	
	//GLSL
	void updateMatrices(unsigned int programId);//Esto cargará las matrices al shader actual
};

#endif
