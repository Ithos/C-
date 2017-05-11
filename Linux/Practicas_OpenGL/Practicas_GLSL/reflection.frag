#version 120

varying vec3 outNormal;//Mediante estas variables pasamos valores a los fragmentos
varying vec2 outUV;
varying vec3 worldVertexPosition;//Posición en el sistema de coordenadas del mundo
varying vec3 worldNormalDirection;//Vector normal en el sistema de coordenadas del mundo

uniform samplerCube cubemap;
uniform vec3 cameraPosition;

void main(){
	vec3 incident=worldVertexPosition-cameraPosition;//Rayo incidente (puede que haya que normalizar)
	vec3 normal= normalize(worldNormalDirection);
	vec3 reflected=reflect(incident,normal);
	gl_FragColor=textureCube(cubemap,reflected);
}
