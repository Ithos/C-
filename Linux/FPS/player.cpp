#include "player.h"

player::player(const char* n,collisionSphere ccs,float ssped,float nspeed,float mspeed,weapon* wep,knife* kn,grenade* g){
	name=n;
	cs=ccs;
	sprintSpeed=ssped;
	normalSpeed=nspeed;
	mousespeed=mspeed;
	gravity.change(0.0,-0.2,0.0);//Podemos ajustar la gravedad cambiando este parámetro. Si no descomentamos esto la gravedad es 0
	setPosition(ccs.getCenter());//La posicion inicial deberia estar por encima del mapa para evitar problemas con la detección de colisiones
	cam.setSpeed(normalSpeed,mousespeed);
	points=0;
	stamina=50;
	health=1000;
	ground=collision=sprint=false;
	weapons.push_back(wep);
	curweapon=0;
	isWeapon=true;
	camBounce=0;
	id=-1;//Id por defecto
	ready=false;//Por defecto indicamos al servidor que el cliente no esta listo
	k=kn;
	if(g){//Quizas en lugar de dar una granada en el constructor podríamos dar un vector para incluir varias al mismo tiempo
		grenades.push_back(g);
		curgrenade=0;
	}else
		curgrenade=-1;
}

player::~player(){
	delete k;//Borramos el cuchillo y las granadas
	for(int i=0;i<grenades.size();i++)delete grenades[i];
}

void player::update(std::vector<collisionPlane>& collplane,const vector3d& newPos,std::vector<enemy*>& enemies){
	direction+=newPos;
	if(direction.getY()>=gravity.getY())direction+=gravity;
	vector3d newpos(cam.getLocation());
	//newpos+=direction;
	vector3d oldpos(newpos);
	int checks=5;
	bool ramp=false;
	for(int j=0;j<checks;j++){//Dividimos la actualización de la posición en 5 pasos para dificultar que se produzca tunneling
		newpos+=direction/checks;
		for(int i=0;i<collplane.size();i++)collision::sphereplane(newpos,collplane[i].getNormal(),collplane[i].getPoints()[0],collplane[i].getPoints()[1],collplane[i].getPoints()[2],collplane[i].getPoints()[3],cs.getR(),true,&ramp);
	}
	if((oldpos.getY()<=newpos.getY())||ramp)ground=true;
	else ground=false;
	setPosition(newpos);
	if(isWeapon)weapons[curweapon]->update();
	if(sprint)stamina-=0.5;
	else if(stamina<50)stamina+=0.05;
	if(sprint&&stamina<=0.0)stopSprint();
	if(cam.getMoving()){
		if(headUp){
			camBounce+=1;
			if(camBounce>=5){
				headUp=false;
			}
		}else{
			camBounce-=1;
			if(camBounce<=-5){
				headUp=true;
			}
		}
		cam.lookAt(cam.getPitch()+(float)camBounce/(15.0-sprint*10),cam.getYaw());
	}
	
	if(k->update())
	{
		haveWeapon(false);
	}else{
		haveWeapon(true);
	}
	
	for(int i=0;i<grenades.size();i++){//Tengo que revisar esto
		if(grenades[i]->update(enemies,collplane))health-=(30.0/(grenades[i]->getPosition()-cs.getCenter()).lenght())*grenades[i]->getDamage();
	}//Creo que queremos comprobar si hay que quitar vida al jugador pero esto tiene pinta de estar mal
	
	direction=vector3d(0,direction.getY(),0);
}

void player::show(){
	glPushMatrix();
	glLoadIdentity();
	if(isWeapon)weapons[curweapon]->show();
	glPopMatrix();
}

void player::setPosition(vector3d pos){
	cs.setCenter(pos);
	cam.setLocation(pos);
}
	
std::string player::getName(){
	return name;
}

int player::getHealth(){
	return health;
}

collisionSphere player::getCollisionSphere(){
	return cs;
}

int player::getPoints(){
	return points;
}
	
void player::setHealth(int n){
	health=n;
}

void player::addPoints(int n){
	points+=n;
}

void player::addWeapon(weapon* wep){
	weapons.push_back(wep);
}

void player::changeWeapon(int n){
	if(weapons.size()>n)curweapon=n;
}

void player::changeWeaponDown(){
	curweapon--;
	if(curweapon<0)curweapon=weapons.size()-1;
}

void player::changeWeaponUp(){
	curweapon++;
	if(curweapon>=weapons.size())curweapon=0;
}

weapon* player::getEquippedWeapon(){
	return weapons[curweapon];
}

void player::haveWeapon(bool b){
	isWeapon=b;
}

void player::setJump(){
	if(ground){
		//El salto es muy rápido, convendría ralentizarlo
		std::cout<<"Jump:"<<direction.getY()<<std::endl;
		//setPosition(cs.getCenter()+vector3d(0,10,0));
		direction.setY(direction.getY()+1.7);
		ground=false;
	}
}

void player::setSprint(){
	if(stamina>2.0){
		sprint=true;
		cam.setSpeed(sprintSpeed,cam.getMousevel());
	}else{
		sprint=false;
		cam.setSpeed(normalSpeed,cam.getMousevel());
	}
}

void player::stopSprint(){
	sprint=false;
	cam.setSpeed(normalSpeed,cam.getMousevel());
}

void player::setId(int i){
	ready=true;
	id=i;
}

int player::getId(){
	return id;
}

bool player::getReady(){
	return ready;
}

int player::getCurframe(){
	return 0; //Esto es temporal para poder probar la función multijugador, pero debería devolver el frame actual del modelo del jugador
}

vector3d player::getRotation(){
	return vector3d(0,cam.getYaw(),0);
}

int player::getWeaponIndex(){
	return curweapon;
}

void player::setKnife(std::vector<enemy*>& enemies){
	int i=k->setKnife(enemies,cam.getVector(),cam.getLocation());//indice del enemigo impactado
	if(i>=0){
		//enemies[i]->decreaseHealth(k->getDamage()); //Ya hacemos daño al enemigo en el metodo del cuchillo
	}
	//Habría que modificar para que afecte a jugadores en multijugador
}

void player::dropGrenade(){
	if(curgrenade!=-1 && curgrenade<grenades.size())
	{
		grenades[curgrenade]->dropGrenade(cam.getVector(),cam.getLocation(),4);
		curgrenade++;
	}
}

std::vector<grenade*>* player::getGrenades(){
	return &grenades;
}

void player::showKnife(){
	k->show();
}
