uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2 pixelSize;

void main(){
	vec2 pos=gl_FragCoord.xy*pixelSize;
	gl_FragColor=vec4(texture2D(texture1,pos).rgb+texture2D(texture2,pos).rgb,1.0);
}
