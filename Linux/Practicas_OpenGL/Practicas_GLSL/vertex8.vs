attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec3 outNormal;//Mediante estas variables pasamos valores a los fragmentos
varying vec3 outTangent;
varying vec3 outColor;
varying vec2 outUV;

varying vec3 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;




void main(){
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	
	position=vec3(modelViewMatrix*vec4(vertex,1.0));
	
	outNormal=normalMatrix*normal;//Transformamos el valor del vector normal del espacio tridimensional al espacio de la ventana
	outTangent=normalMatrix*tangent;//Lo mismo con los vectores tangentes
	outColor=color;//Estos valores simplemente se interpolan entre vertices
	outUV=UV;
}
