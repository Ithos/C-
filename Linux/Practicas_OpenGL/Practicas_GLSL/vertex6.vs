attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec3 outNormal;//Mediante estas variables pasamos valores a los fragmentos
varying vec3 outTangent;
varying vec3 outColor;
varying vec2 outUV;

void main(){
	gl_Position=gl_ModelViewProjectionMatrix*vec4(vertex,1.0);
	
	outNormal=gl_NormalMatrix*normal;//Transformamos el valor del vector normal del espacio tridimensional al espacio de la ventana
	outTangent=gl_NormalMatrix*tangent;//Lo mismo con los vectores tangentes
	outColor=color;//Estos valores simplemente se interpolan entre vertices
	outUV=UV;
}
