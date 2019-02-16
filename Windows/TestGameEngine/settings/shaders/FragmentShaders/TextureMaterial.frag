#version 330 core

uniform sampler2D texture;

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormalPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out  vec4 WorldPosOut;
layout (location = 2) out vec4 NormalOut; 
layout (location = 3) out vec4 TexCoordOut;

void main(void)
{
	FragColor = vec4(texture(texture, fragTexCoord).xyz, 1.0);
	WorldPosOut = vec4(fragNormalPos, 1.0);
    NormalOut = vec4(normalize(fragNormal), 1.0);
    TexCoordOut = vec4(fragTexCoord, 0.0, 1.0);
}