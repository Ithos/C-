#version 120

varying vec3 outNormal;//Mediante estas variables pasamos valores a los fragmentos
varying vec2 outUV;
varying vec3 worldVertexPosition;//Posición en el sistema de coordenadas del mundo
varying vec3 worldNormalDirection;//Vector normal en el sistema de coordenadas del mundo

uniform samplerCube cubemap;
uniform vec3 cameraPosition;

void main(){
	float n1=1,n2=1.4;
	vec3 incident=worldVertexPosition-cameraPosition;//Rayo incidente (puede que haya que normalizar)
	vec3 normal= normalize(worldNormalDirection);
	//Calculamos los vectores refractado y reflejado
	vec3 refracted=refract(incident,normal,n2);
	vec3 reflected=reflect(incident,normal);
	//Calculamos el cos de los angulos con la normal
	vec3 nIncident=normalize(incident);
	vec3 nRefracted=normalize(refracted);
	float cosi=dot(nIncident,normal);
	float cost=dot(nRefracted,normal);
	//Calculamos el coeficiente de reflexión
	float Rp=((n1*cost-n2*cosi)/(n1*cost+n2*cosi))*((n1*cost-n2*cosi)/(n1*cost+n2*cosi));
	float Rs=((n1*cosi-n2*cost)/(n1*cosi+n2*cost))*((n1*cosi-n2*cost)/(n1*cosi+n2*cost));
	float R=(Rs+Rp)/2;
	//Aplicamos los coeficientes a los vectores
	gl_FragColor=textureCube(cubemap,R*reflected+(1-R)*refracted);
}
