#version 120 //Esto significa que usamos la versión de GLSL 1.2

uniform vec3 color;//El modificador uniform en una variable global permite pasar información entre el programa y el shader
varying vec3 color2;//Este modificador nos permite pasar datos de vertex a fragment, para esto es importante que ambas variables se llamen igual. En la version 1.3 este modificador se ha renombrado como out en vertex y como in en fragment
varying vec2 texcoord; //Aqui almacenaremos las coordenadas de la textura
attribute vec3 attr;//Este modificador permite recibir datos del programa cada frame, en la versión 1.3 se ha renombrado como in

void main(){//Tiene que haber un único main por shader
	gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;//Esto básicamente multiplica la escena por la matriz de transformación actual //Todas estas cosas se han eliminado a partir de la versión 1.3 de GLSL
	color2=attr;
	texcoord=gl_MultiTexCoord0.xy;// Este parámetro almacena el valor de las coordenadas de la textura para el vertice actual, el parámetro almacena un vector de 4 dimensiones, pero solo nos quedamos con 2
}
