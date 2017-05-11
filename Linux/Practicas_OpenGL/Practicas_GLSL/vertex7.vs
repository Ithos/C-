attribute vec3 vertex;
attribute vec2 UV;

varying vec2 outUV;

void main(){
	gl_Position=gl_ModelViewProjectionMatrix*vec4(vertex,1.0);
	outUV=UV;
}
