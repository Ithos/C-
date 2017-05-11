//uniform sampler2DShadow shadowMap; //Esto es para shadow2DProj
uniform sampler2D shadowMap;
uniform vec3 cameraPosition;

varying vec4 lightVertexPosition;
varying vec3 outColor;
varying vec3 position;
varying vec3 outNormal;
varying vec3 lightPosition;

const vec3 ambientColor=vec3(0.05,0.05,0.05);//Componentes de la luz
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);

void main(){
	//float shadowValue=shadow2DProj(shadowMap,lightVertexPosition).r;//==1 iluminado/==0 en sombra
	//En algunas versiones de openGl la función de arriba no funciona, por lo que incluimos una implementación manual
	/*float shadowValue;
	vec4 lightVertexPosition2=lightVertexPosition;
	lightVertexPosition2/=lightVertexPosition.w;
	if(texture2D(shadowMap,lightVertexPosition2.xy).r>=lightVertexPosition2.z){
		shadowValue=1.0;
	}else{
		shadowValue=0.0;
	}*/ 
	//Version con filtro para difuminar bordes
	float shadowValue=0.0;
	vec4 lightVertexPosition2=lightVertexPosition;
	lightVertexPosition2/=lightVertexPosition.w;
	for(float x=-0.001;x<=0.001;x+=0.0005){//Deberiamos usar el tamaño del pixel de la imagen
		for(float y=-0.001;y<=0.001;y+=0.0005){
			if(texture2D(shadowMap,lightVertexPosition2.xy+vec2(x,y)).r>=lightVertexPosition2.z){
		shadowValue+=1.0;
	}
		}
	}
	shadowValue/=16.0;
	
	vec3 normal=normalize(outNormal);
	vec3 surf2light=normalize(lightPosition-position);
	vec3 surf2camera=normalize(-position);
	vec3 reflection=-reflect(surf2camera,normal);
	float diffuseContribution=max(0.0,dot(normal,surf2light));
	float specularContribution=pow(max(0.0,dot(reflection,surf2light)),16.0);
	
	gl_FragColor=vec4(ambientColor*outColor+(shadowValue+0.005)*diffuseContribution*diffuseColor*outColor+(shadowValue<0.5 ? vec3(0.0,0.0,0.0) : specularContribution*specularColor*shadowValue),1.0);
}
