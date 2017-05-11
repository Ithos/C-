varying vec3 outNormal;//Mediante estas variables pasamos valores a los fragmentos
varying vec3 outTangent;
varying vec3 outColor;
varying vec2 outUV;

uniform sampler2D texture0; //Pueden ser necesarias mas variables de estas para manejar más texturas

void main(){
	
	gl_FragColor=vec4(outColor,1.0);
	
}
