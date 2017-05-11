#version 120 //Esto significa que usamos la versión de GLSL 1.2
varying vec3 position;
varying vec3 normal;
 
varying vec2 texcoord; //Aqui almacenaremos las coordenadas de la textura

void main(){//Tiene que haber un único main por shader
	gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;//Esto básicamente multiplica la escena por la matriz de transformación actual //Todas estas cosas se han eliminado a partir de la versión 1.3 de GLSL
	position=vec3(gl_ModelViewMatrix*gl_Vertex);//Posición actual
	normal=gl_NormalMatrix*gl_Normal;//Vector normal 
	texcoord=gl_MultiTexCoord0.xy;// Este parámetro almacena el valor de las coordenadas de la textura para el vertice actual, el parámetro almacena un vector de 4 dimensiones, pero solo nos quedamos con 2
}
