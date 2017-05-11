#version 120 //Esto significa que usamos la versión de GLSL 1.2

uniform vec3 color;//El modificador uniform en una variable global permite pasar información entre el programa y el shader
varying vec3 color2;//Este tipo de datos interpola su valor si es distinto en dos vertices

void main(){
	gl_FragColor=vec4(color2,1.0);//Devolvemos el color del píxel que en este caso lo damos como rojo
}
