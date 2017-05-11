uniform sampler2D texture;
uniform vec2 pixelSize;
uniform int isVertical;//Usamos esta variable para saber si estamos realizando un  difuminado vertical u horizontal

void main(){
	vec2 pos=gl_FragCoord.xy*pixelSize;//Esto es un truco para ahorrar recursos cuando empleamos un modelo dibujado sobre una textura
	vec4 result=vec4(0.0);
	
	float value[9];
	value[0]=0.07511360795411207;//Valores de la función gaussiana
	value[1]=0.12384140315297386;
	value[2]=0.07511360795411207;
	value[3]=0.12384140315297386;
	value[4]=0.20417995557165622;
	value[5]=0.12384140315297386;
	value[6]=0.07511360795411207;
	value[7]=0.12384140315297386;
	value[8]=0.07511360795411207;
	
	
	if(isVertical==1){
		vec2 curPos=vec2(pos.x,pos.y-4.0*pixelSize.y);
		for(int i=0;i<9;i++){
			result+=texture2D(texture,curPos)*value[i];
			curPos.y+=pixelSize.y;
		}
	}else{
		vec2 curPos=vec2(pos.x-4.0*pixelSize.x,pos.y);
		for(int i=0;i<9;i++){
			result+=texture2D(texture,curPos)*value[i];
			curPos.x+=pixelSize.x;
		}
	}
	
	gl_FragColor=vec4(result.rgb,1.0);//Todo esto da como resultado un efecto de difuminado, sobretodo en los bordes (que es donde se requiere)
	//Este sistema no proporciona mucho difuminado, por lo que hay que emplear un segundo buffer para pasar el filtro de difuminado varias veces
	//Para mejorar el rendimiento de los efectos de difuminado se debería emplear un algoritmo que detecte bordes
}
