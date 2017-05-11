uniform sampler2D texture;
uniform vec2 pixelSize;

void main(){
	vec2 pos=gl_FragCoord.xy*pixelSize;//Esto es un truco para ahorrar recursos cuando empleamos un modelo dibujado sobre una textura
	vec4 result=vec4(0.0);
	result+=texture2D(texture,vec2(pos.x-pixelSize.x,pos.y-pixelSize.y));
	result+=2.0*texture2D(texture,vec2(pos.x,pos.y-pixelSize.y));
	result+=texture2D(texture,vec2(pos.x+pixelSize.x,pos.y-pixelSize.y));
	result+=2.0*texture2D(texture,vec2(pos.x-pixelSize.x,pos.y));
	result+=3.0*texture2D(texture,vec2(pos.x,pos.y));
	result+=2.0*texture2D(texture,vec2(pos.x+pixelSize.x,pos.y));
	result+=texture2D(texture,vec2(pos.x-pixelSize.x,pos.y+pixelSize.y));
	result+=2.0*texture2D(texture,vec2(pos.x,pos.y+pixelSize.y));
	result+=texture2D(texture,vec2(pos.x+pixelSize.x,pos.y+pixelSize.y)); //Vamos a sumar los colores de los pixeles alrededor de cada pixel dando un peso a cada uno
	result/=15.0;//Luego dividimos el resultado
	gl_FragColor=vec4(result.rgb,1.0);//Todo esto da como resultado un efecto de difuminado, sobretodo en los bordes (que es donde se requiere)
	//Este sistema no proporciona mucho difuminado, por lo que hay que emplear un segundo buffer para pasar el filtro de difuminado varias veces
	//Para mejorar el rendimiento de los efectos de difuminado se debería emplear un algoritmo que detecte bordes
	//gl_FragColor=vec4(1.0,0.0,0.0,1.0);
}
