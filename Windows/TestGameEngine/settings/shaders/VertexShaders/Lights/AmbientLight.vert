#version 330 core
layout (location = 0) in vec4 posAttr; // the position variable has attribute position 0
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 aNormal;

uniform highp mat4 modelViewProjectionMatrix;
uniform highp mat4 modelViewMatrix;
uniform highp mat4 modelMatrix;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormalPos;

void main()
{
	gl_Position = modelViewProjectionMatrix * posAttr;

	fragPos = vec3(modelViewMatrix * posAttr);
	fragNormal = (modelMatrix * vec4(aNormal,0.0)).xyz;//Transformamos el valor del vector normal del espacio tridimensional al espacio de la ventana
	fragTexCoord = TexCoord;
	fragNormalPos = (modelMatrix * vec4(posAttr.x, posAttr.y, posAttr.z, 1.0)).xyz;
}