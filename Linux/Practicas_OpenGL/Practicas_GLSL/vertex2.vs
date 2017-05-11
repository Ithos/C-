#version 120 //Esto significa que usamos la versión de GLSL 1.2

uniform vec3 color;//El modificador uniform en una variable global permite pasar información entre el programa y el shader
varying vec3 color2;//Este modificador nos permite pasar datos de vertex a fragment, para esto es importante que ambas variables se llamen igual. En la version 1.3 este modificador se ha renombrado como out en vertex y como in en fragment
attribute vec3 attr;//Este modificador permite recibir datos del programa cada frame, en la versión 1.3 se ha renombrado como in

void main(){//Tiene que haber un único main por shader
	gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;//Esto básicamente multiplica la escena por la matriz de transformación actual //Todas estas cosas se han eliminado a partir de la versión 1.3 de GLSL
	float f=float(10);//Para convertir un tipo de dato a otro en GLSL lo recomendado es emplear el constructor de la clase
	vec3 vec=vec3(1.0,1.0,1.0);//En GLSL existen tipos de datos para representar vectores de 2,3 y 4 dimensiones
	ivec3 vect=ivec3(1,1,1);//Los vectores por defecto emplean la precisión float, pero se puede indicar una precisión de int, tambien se puede crear un vector de boolean mediante el tipo bvec
	mat3 mat;//En GLSL también hay tipos que permiten declarar matrices indicando mat y las dimensiones de la matriz ej:mat2x3 o mat4 si es cuadrada
	mat[0]=vec3(1.0,2.0,3.0);//De esta forma accedemos a la primera COLUMNA de la matriz, ya que en GLSL se emplea la matriz traspuesta
	mat[2][1]=8;//Asi accedemos al elemento de la tercera columna y la segunda fila
	//En GLSL se puede acceder a los datos de un vector como componentes x,y,x,w de esta forma color.x es equivalente a color[0], de esta forma color.xx es equivalente a vec2=(color.x,color.x) tambien pueden emplearse de la misma forma las coordenadas rgb para colores y stpq para texturas en cualquier vector
	color2=attr;
}
