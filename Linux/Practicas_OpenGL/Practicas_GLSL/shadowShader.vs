attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 color;



uniform mat4 modelViewProjectionMatrix;
uniform mat4 lightModelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 lightMatrix; //ModelViewMatrix para el foco
uniform vec3 inLightPosition;//Sistema de coordenadas del modelo
uniform mat3 normalMatrix;

varying vec4 lightVertexPosition;
varying vec3 outColor;
varying vec3 position;
varying vec3 outNormal;
varying vec3 lightPosition; //Sistema de coordenadas de la escena 

void main(){
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	lightVertexPosition=lightModelViewProjectionMatrix*vec4(vertex,1.0);
	position=vec3(modelViewMatrix*vec4(vertex,1.0));
	outNormal=normalMatrix*normal;//Transformamos el valor del vector normal del espacio tridimensional al espacio de la ventana
	outColor=color;//Estos valores simplemente se interpolan entre vertices
	lightPosition=vec3(lightMatrix*vec4(inLightPosition,1.0));//Esta transformación mantiene el foco de luz siempre en las mismas coordenadas en el sistema de coordenadas de la escena
}
