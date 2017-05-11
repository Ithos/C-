#include "enemy.h"

enemy::enemy(std::vector<unsigned int>& anim,unsigned int w,unsigned int a,unsigned int d,int h,int str,float sp,collisionSphere css){
	frames=anim;
	walk=w;
	attack=a;
	die=d;
	strength=str;
	speed=sp;
	cs=css;
	walking=true;
	attacking=dead=false;
	health=h;
	curframe=0;//Esto evita segmentation errors
}

enemy::~enemy(){
	
}

bool enemy::update(std::vector<collisionPlane>& col,vector3d playerPos){
	if(!dead){//Comprobamos si el enemigo esta muerto
		direction.change(playerPos-cs.getCenter());//Calculamos la direccion desde el enemigo al jugador
		direction.normalize();
		
		vector3d newpos(cs.getCenter());
		newpos.setY(newpos.getY()-0.3);//esto hay que activarlo para que afecte la gravedad
		if(!attacking)newpos+=direction*speed;
		for(int i=0;i<col.size();i++){//Comprobamos colisiones despues de mover
			collision::sphereplane(newpos,col[i].getNormal(),col[i].getPoints()[0],col[i].getPoints()[1],col[i].getPoints()[2],col[i].getPoints()[3],cs.getR(),true);
		}
			setLocation(newpos);
			
			rotation.setY(std::acos(direction.getX()));//Ajustamos la rotación para que la imagen del enemigo apunte hacia el jugador
			if(direction.getZ()<0)rotation.setY(-rotation.getY());
	}
	if(health<=0&&!dead){
		std::cout<<"enemy dead"<<std::endl;
		dead=true;
		walking=false;
		attacking=false;
		curframe=walk+attack;
		dying=SDL_GetTicks();
		return true;
	}
	curframe++;//Avanzamos y manejamos la animación
	if(walking&&curframe>=walk)curframe=0;
	else if(attacking&&curframe>=walk+attack)curframe=walk;
	else if(dead&&curframe>=walk+attack+die)curframe=walk+attack+die-1;
	return false;
}

void enemy::show(){
	glPushMatrix();
		glTranslatef(cs.getCenter().getX(),cs.getCenter().getY(),cs.getCenter().getZ());
		glRotatef(-rotation.getX()*(180.0/M_PI),1,0,0);
		glRotatef(-rotation.getY()*(180.0/M_PI),0,1,0);
		glRotatef(-rotation.getZ()*(180.0/M_PI),0,0,1);
		glScalef(1.0,1.0,1.0);
		glCallList(frames[curframe]);
	glPopMatrix();
}

bool enemy::setAttack(collisionSphere player){
	if(!dead&&collision::spheresphere(cs.getCenter(),cs.getR(),player.getCenter(),player.getR())){
		walking=false;//Si hay colision deja de andar y ataca
		attacking=true;
		return true;
	}else{
		if(!dead)walking=true;
		attacking=false;
		return false;
	}
}

collisionSphere* enemy::getCollisionSphere(){
	return &cs;
}

void enemy::setLocation(vector3d newLoc){
	cs.setCenter(newLoc);
}

void enemy::decreaseHealth(int num){
	health-=num;
}

int enemy::getHealth(){
	return health;
}

int enemy::getStrength(){
	return strength;
}

bool enemy::isDead(){
	return dead;
}

bool enemy::disappear(){
	return (dead && SDL_GetTicks()-dying>5000);//Comprobamos si el enemigo murió hace 5 secs o más y en tal caso damos true
}
