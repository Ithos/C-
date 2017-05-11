#version 120

attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec3 outNormal;//Mediante estas variables pasamos valores a los fragmentos
varying vec2 outUV;
varying vec3 worldVertexPosition;//Posición en el sistema de coordenadas del mundo
varying vec3 worldNormalDirection;//Vector normal en el sistema de coordenadas del mundo

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main(){
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	worldVertexPosition=vec3(modelMatrix*vec4(vertex,1.0));
	worldNormalDirection=mat3(modelMatrix)*normal;
}
