#version 120 //Esto significa que usamos la versión de GLSL 1.2

uniform sampler2D img,img2; //Este tipo de variable sirve para almacenar imagenes en 2D
uniform vec3 color;//El modificador uniform en una variable global permite pasar información entre el programa y el shader
varying vec3 color2;//Este tipo de datos interpola su valor si es distinto en dos vertices
varying vec2 texcoord; //Esto es para recibir las coordenadas de la textura

void main(){
	vec4 texcolor=texture2D(img,texcoord);//Esta función carga la textura con todas las opciones que se hayan seleccionado
	vec4 texcolor2=texture2D(img2,texcoord);
	//gl_FragColor=texcolor;//Devolvemos el color del píxel que en este será el de la textura //Así cargamos una textura
	gl_FragColor=0.3*texcolor + 0.7*texcolor2;//Si tenemos varias texturas podemos combinarlas
}
