#version 330                                                                        

layout (location = 0) in vec4 posAttr; 

uniform mat4 modelViewProjectionMatrix;

void main()
{          
    gl_Position = modelViewProjectionMatrix * posAttr;
}