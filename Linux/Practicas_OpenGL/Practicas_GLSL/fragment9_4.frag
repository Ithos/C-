uniform sampler2D texture;
uniform vec2 pixelSize;

void main(){
	vec2 pos=gl_FragCoord.xy*pixelSize;//Esto es un truco para ahorrar recursos cuando empleamos un modelo dibujado sobre una textura
	
	float intensity=dot(texture2D(texture,pos).rgb,vec3(0.29,0.59,0.12));//Si se quiere un mejor efecto hay que tener en cuenta la desviación del ojo al detectar colores
	if(intensity>0.99){
		gl_FragColor=vec4(1.0,1.0,1.0,1.0);
	}else{
		gl_FragColor=vec4(0.0,0.0,0.0,1.0);
	}
	
	
}
