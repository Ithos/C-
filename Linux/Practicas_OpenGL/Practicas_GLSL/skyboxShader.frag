varying vec3 outColor;
uniform samplerCube cubemap;
varying vec3 vertexVector;

void main(){
	gl_FragColor=textureCube(cubemap,vertexVector);
}
