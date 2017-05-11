uniform sampler2D texture;
uniform vec2 pixelSize;

void main(){
	vec2 pos=gl_FragCoord.xy*pixelSize;//Esto es un truco para ahorrar recursos cuando empleamos un modelo dibujado sobre una textura
	
	//Una de las cosas que podemos hacer al pos-procesar la imagen es convertirla a blanco y negro
	//float intensity=dot(texture2D(texture,pos).rgb,vec3(0.33333,0.33333,0.33333));//El segundo vector suma todos los colores y los divide entre tres
	
	float intensity=dot(texture2D(texture,pos).rgb,vec3(0.29,0.59,0.12));//Si se quiere un mejor efecto hay que tener en cuenta la desviación del ojo al detectar colores
	
	
	gl_FragColor=vec4(intensity,intensity,intensity,1.0);
}
