#include "particlesystem.h"

particlesystem::particle::particle(){
	lastUpdate=SDL_GetTicks();
}

particlesystem::particle::particle(const vector3d& pos,const vector3d& rot,const vector3d& col,const vector3d& sp,float s){
	position=pos;
	rotation=rot;
	color=col;

	size=s;
	speed=sp;
	lastUpdate=SDL_GetTicks();
	dead=false;//Indica si la partícula ha desaparecido o no
}

bool particlesystem::particle::update(){
	float deltaTime=(float)(SDL_GetTicks()-lastUpdate)/1000.0;//Esto nos indica cuanto tiempo ha pasado desde la última actualización (en ms)
	position+=(speed*deltaTime);//Actualizamos la posición
	lifetime-=(SDL_GetTicks()-lastUpdate);//Reducimos el tiempo de vida
	lastUpdate=SDL_GetTicks();//Guardamos el tiempo actual
	float perc=(float)lifetime/totalLifetime;//Indica que porcentaje del tiempo de vida ha pasado
	color=startcolor*perc + endcolor*(1-perc);
	if(lifetime<=0){//Comprobamos si la particula ha desaparecido
		dead=true;
		return true;
	}
	return false;
}

void particlesystem::particle::show(camera& cam){
	glColor4f(color.getX()/255.0,color.getY()/255.0,color.getZ()/255.0,(float)lifetime/totalLifetime);//En el vector empleamos valores de 0 a 255, la transparencia aumenta cuando se reduce el tiempo de vida
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(position.getX(),position.getY(),position.getZ());
			glTexCoord2f(1,0);
			glVertex3f(position.getX(),position.getY()+size,position.getZ());//Size es el tamaño de la particula, al sumarlo representamos la partícula como un cuadrado
			glTexCoord2f(1,1);
			glVertex3f(position.getX()+size,position.getY()+size,position.getZ());
			glTexCoord2f(0,1);
			glVertex3f(position.getX()+size,position.getY(),position.getZ());
		glEnd();
}

particlesystem::particlesystem(int db,const vector3d& pos,float ms,float mas,float minSp,float maxSp,const vector3d& f,const vector3d& startcolor,const vector3d& endcolor,int minlt,int maxlt,int nm,unsigned int tex){
	std::cout<<"particle system constructor"<<std::endl;
	particleNum=db;
	position=pos;
	minSize=ms;
	maxSize=mas;
	minSpeed=minSp;
	maxSpeed=maxSp;
	force=f;
	startColor=startcolor;
	endColor=endcolor;
	minLifetime=minlt;
	maxLifetime=maxlt;
	isActive=true;
	if(tex==-1){//Tex tiene un valor por defecto de -1, si no espera un unsigned int que indique la textura a usar
		useTexture=false;
	}else{
		useTexture=true;
		texture=tex;
	}
	numparticles=nm;
	std::cout<<"creating particles"<<std::endl;
	for(int i=0;i<db && numparticles>0;i++)
	{
		particles.push_back(particle());
		generateParticle(particles[i]);
	}
}

particlesystem::~particlesystem(){//Destrucor vacio
	
}

void particlesystem::generateParticle(particle& p){
	p.position=position;
	p.size=(float)(rand()%((int)((maxSize-minSize)*1000)))/1000.0 + minSize;//Damos un tamaño aletorio a las partículas comprendido entre el tamaño máximo y el mínimo
	p.speed=vector3d(((float)(rand()%((int)((maxSpeed-minSpeed))*1000)/1000.0 + minSpeed)-(maxSpeed-minSpeed)/2)+force.getX(),((float)(rand()%((int)((maxSpeed-minSpeed))*1000)/1000.0 + minSpeed)-(maxSpeed-minSpeed)/2)+force.getY(),((float)(rand()%((int)((maxSpeed-minSpeed))*1000)/1000.0 + minSpeed)-(maxSpeed-minSpeed)/2)+force.getZ());//Al actualizar la velocidad tenemos una parte que depende de la fuerza que este actuando en el mapa y otra parte aleatoria dentro de unos límites
	p.totalLifetime=p.lifetime=rand()%((maxLifetime-minLifetime)*1000)/1000.0 + minLifetime;
	p.startcolor=startColor;
	p.endcolor=endColor;
	numparticles--;
}

void particlesystem::setPosition(const vector3d& pos){
	position=pos;
}

void particlesystem::update(){
	for(int i=0;i<particles.size();i++){
		if(particles[i].update() && numparticles>0)generateParticle(particles[i]);//Si una partícula desaparece, pero aún quedan partículas en el sistema se genera una nueva partícula
	}
	if(numparticles<=0)isActive=false;//Si no quedan partículas el sistema queda desactivado
}

void particlesystem::show(camera& cam){
	float f[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,f);
	for(int i=0;i<3;i++){//Esto es un truco para modificar la matriz de proyección de tal forma que las partículas siempre apunten hacía la camara
		for(int j=0;j<3;j++){
			if(i!=j)
				f[i*4+j]=0;
			else
				f[i*4+j]=1;
		}
	}
	glPushMatrix();
	glLoadMatrixf(f);
	if(useTexture){
		glBindTexture(GL_TEXTURE_2D,texture);//Incluimos la textura
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//Damos unos cuantos parámetros
		glTexEnvi(GL_TEXTURE_ENV_MODE,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	}
	for(int i=0;i<particles.size();i++)particles[i].show(cam);
	glPopMatrix();
	if(useTexture){
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}

bool particlesystem::isParticleSystemActive(){
	return isActive;
}

void particlesystem::setNumParticles(int n){
	numparticles=n;
	isActive=true;
	particles.resize(n);//cambiamos el tamaño del vector
	std::cout << "resized to " << n << std::endl;
	for(int i=0;i<n;i++)generateParticle(particles[i]);//Generamos particulas
}

void particlesystem::setForce(const vector3d& f){
	force=f;
}
