uniform sampler2D texture;
uniform vec2 pixelSize;

void main(){
	vec2 pos=gl_FragCoord.xy*pixelSize;//Esto es un truco para ahorrar recursos cuando empleamos un modelo dibujado sobre una textura
	
	
	gl_FragColor=texture2D(texture,pos);
}
