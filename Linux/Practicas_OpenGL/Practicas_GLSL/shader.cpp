#include "shader.h"

shader::shader(const char *vss,const char *fss){
	std::string source;
	loadFile(vss,source);//Cargamos el código fuente
	vs=loadShader(source,GL_VERTEX_SHADER);//cargamos el shader y almacenamos su id
	source="";//Borramos source
	loadFile(fss,source);
	fs=loadShader(source,GL_FRAGMENT_SHADER);
	
	program=glCreateProgram();//Creamos un programa vacío
	glAttachShader(program,vs);//agregamos shader
	glAttachShader(program,fs);
	
	glLinkProgram(program);//Linkamos los archivos compilados
	glUseProgram(program);//Usamos el programa
}

shader::~shader(){
	glDetachShader(program,vs);
	glDetachShader(program,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
}

void shader::loadFile(const char* fn,std::string &str){//Esta función carga un archivo y lo almacena en str
	std::ifstream in(fn);
	if(!in.is_open()){
		std::cout << "The file " << fn << "could not be opened." << std::endl;
		return;
	}
	char tmp[1000];
	while(!in.eof()){
		in.getline(tmp,1000);
		str+=tmp;
		str+='\n';
	}
	
}
unsigned int shader::loadShader(std::string &source,unsigned int mode){//Esta función compila los shaders y emite los errores de compilación
	unsigned int id;
	id=glCreateShader(mode);
	
	const char* csource=source.c_str();//Convertimos el código fuente de source a strings con el estilo de c
	
	glShaderSource(id,1,&csource,NULL);//introducimos el código
	glCompileShader(id);//Compilamos el shader
	
	char error[1000];
	glGetShaderInfoLog(id,1000,NULL,error);
	std::cout << "Compile status:\n" << error << std::endl;
	
	return id;
}
void shader::useShader(){
	glUseProgram(program);
}

unsigned int shader::getProgramId(){
	return program;
}

void shader::delShader(){
	glUseProgram(0);
}
