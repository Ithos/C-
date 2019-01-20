#version 330 core
layout (location = 0) in vec4 posAttr; // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;

uniform highp mat4 modelViewProjectionMatrix;
uniform highp mat4 modelViewMatrix;
uniform highp mat3 normalMatrix;

out vec3 fragNormal;
out vec3 fragPos;

void main()
{
	gl_Position = modelViewProjectionMatrix * posAttr;

	fragPos = vec3(modelViewMatrix * posAttr);
	fragNormal = normalMatrix * aNormal;//Transformamos el valor del vector normal del espacio tridimensional al espacio de la ventana
}