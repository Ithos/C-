#version 120 //Esto significa que usamos la versión de GLSL 1.2
varying vec3 position;
varying vec3 normal;
 
varying vec2 texcoord; //Aqui almacenaremos las coordenadas de la textura

uniform vec3 lightPos;
varying vec3 tangentSurface2Light;
varying vec3 tangentSurface2View;

void main(){//Tiene que haber un único main por shader
	gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;//Esto básicamente multiplica la escena por la matriz de transformación actual //Todas estas cosas se han eliminado a partir de la versión 1.3 de GLSL
	position=vec3(gl_ModelViewMatrix*gl_Vertex);//Posición actual
	normal=gl_NormalMatrix*gl_Normal;//Vector normal 
	texcoord=gl_MultiTexCoord0.xy;// Este parámetro almacena el valor de las coordenadas de la textura para el vertice actual, el parámetro almacena un vector de 4 dimensiones, pero solo nos quedamos con 2
	
	vec3 tangent;//Aqui calculamos vectores tangentes, normales y binormales, esta no es la mejor solución, deberian calcularse cuando cargamos el objeto y recibirlos como atributo
	vec3 v1=cross(gl_Normal,vec3(0.0,0.0,1.0));
	vec3 v2=cross(gl_Normal,vec3(0.0,1.0,0.0));//cross es el producto vectorial
	if(length(v1)>length(v2)) tangent=v1;
	else tangent=v2; //no calculamos exactamente el vector tangente, si no que lo aproximamos quedandonos con el mayor vector de ambos
	
	vec3 n=normalize(gl_NormalMatrix*gl_Normal);
	vec3 t=normalize(gl_NormalMatrix*tangent);
	vec3 b=cross(n,t);
	//mat3 mat=mat3(t.x,b.x,n.x,t.y,b.y,n.y,t.z,b.z,n.z);//Matriz de transformación al espacio tangente //Si se emplea esta versión de la matrz de rotación creo que hay que multiplicar v1 y v2 por -1
	mat3 mat=mat3(t.x,t.y,t.z,b.x,b.y,b.z,n.x,n.y,n.z);//No se si este es el correcto o el correcto es el de arriba
	
	vec3 vector=normalize(lightPos-position);//Calculamos el vector de la posición de la superficie al foco
	tangentSurface2Light=mat*vector;//Transformamos el vector multiplicandole por la matriz
	
	vector=normalize(-position);//Hacemos lo mismo con el vector 
	tangentSurface2View=mat*vector;
	
}
