#version 120 //Esto significa que usamos la versión de GLSL 1.2

uniform sampler2D img;//Aqui almacenamos la textura
varying vec2 texcoord; //Esto es para recibir las coordenadas de la textura

uniform sampler2D normMap;//Aqui recibimos el normal map

varying vec3 position;
varying vec3 normal;

uniform vec3 lightPos;//Posición de la fuente de luz //Si no queremos que el foco se mueva con la camara hay que multiplicarlo por gl_ModelViewMatrix

uniform vec3 mAmbient;//Estos valores para las propoedades del material se pueden sacar de gl_FrontMaterial.ambient|diffuse|especular|shininess, pero esto deja de ser válido a partir de GLSL 1.3
uniform vec3 mDiffuse;
uniform vec3 mSpecular;
uniform float shininess;

uniform vec3 lAmbient;//Estos valores para las propoedades de la luz se pueden sacar de gl_LightSource[].ambient|diffuse|especular|shininess, pero esto deja de ser válido a partir de GLSL 1.3
uniform vec3 lDiffuse;
uniform vec3 lSpecular;

varying vec3 tangentSurface2Light;//Estas variables nos servirán para recibir los vectores en el espacio tangente
varying vec3 tangentSurface2View;

void main(){
	
	vec4 texcolor=texture2D(img,texcoord);//Esta función carga la textura con todas las opciones que se hayan seleccionado
	
	float dist=length(position-lightPos);//Distancia de la superficie a la fuente de luz
	float att=1.0/(1.0+1.0*dist+0.1*dist*dist);//Atenuación de la luz según la distancia
	
	vec3 ambient=vec3(texcolor)*lAmbient;//Componente ambiental de la luz
	
	vec3 surf2light=normalize(tangentSurface2Light);//Vector de la superficie a la fuente de luz //Cambiamos el vector en el espacio normal por el transformado al espacio tengente
	vec3 norm=normalize(texture2D(normMap,texcoord).xyz*2.0-1.0);//Calculamos el nuevo vector normal y  lo transformamos al rango -1.0 1.0
	float dContribution=max(0.0,dot(norm,surf2light));//Calculamos la contribución de la componente difusa de la luz calculando el coseno del angulo entre surf2light y la normal
	vec3 diffuse= dContribution*(vec3(texcolor)*lDiffuse);//Componente difusa de la luz
	
	vec3 surf2view = normalize(tangentSurface2View);//un vector normal de la superficie a la posición del punto de vista //De nuevo cambiamos el vector por el transformado al espacio tangente
	vec3 reflection=reflect(-surf2light,norm);//Invertimos especularmente la dirección del vector surf2view para obtener el vector de reflexión
	float sContribution=pow(max(0.0,dot(surf2view,reflection)),shininess);//Contribución de la componente especular de la luz
	vec3 specular=sContribution*lSpecular*mSpecular;//Componente especular de la luz //Si se sustituye mSpecular por vec3(texcolor) la superficie es menos reflectante
	
	gl_FragColor=vec4((ambient+diffuse+specular)*att,1.0);//Sumamos todas las componentes
}
