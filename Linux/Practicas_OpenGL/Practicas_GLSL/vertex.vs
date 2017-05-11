
void main(){//Tiene que haber un único main por shader
	gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;//Esto básicamente multiplica la escena por la matriz de transformación actual
}
