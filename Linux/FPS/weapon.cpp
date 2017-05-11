#include "weapon.h"

weapon::weapon(std::vector<unsigned int>& anim,unsigned int ol,unsigned int na,unsigned int fa,unsigned int ra,vector3d pos,vector3d rot,vector3d apos,vector3d arot,float prec,float aprec,int dmg,int maxb,int allbul,int sp,const char* nam,bool aut,const vector3d& partPos,particlesystem* part,float cs){
	frames=anim;
	outerview=ol;
	normalanimation=na;
	fireanimation=fa;
	reloadanimation=ra;
	precision=(prec!=0?prec:0.0000001);
	aimprecision=(aprec!=0?aprec:0.0000001);
	
	position=pos;
	rotation=rot;
	
	aimposition=apos;
	aimrotation=arot;
	
	damage=dmg;
	allBullets=allbul;
	maxBullets=maxb;
	speed=sp;
	name=nam;
	
	expected_pos=position;
	expected_rot=rotation;
	
	aiming=false;
	reloading=false;
	automatic=aut;
	fired=false;
	isTest=false;//Esto debería ser false cuando terminamos de probar las posiciones de las armas en la pantalla
	
	bullets=maxBullets;//Inicializamos la munición actual al máximo del cargador
	lastshot=1000;//Damos un valor grande para inicializar
	curframe=0;//Empezamos en el primer frame
	curmode=1;//Modo normal
	
	changeSpeed=cs;
	particlePosition=partPos;
	particles=part;
	particles->setNumParticles(0);
	
}

weapon::~weapon(){
	for(int i=0;i<attachments.size();i++)delete attachments[i];
}
	
void weapon::update(){
	if(attachments.size()>0)test(attachments[1]);
	lastshot++;
	curframe++;
	if(curmode==1){
		if(curframe>=normalanimation)curframe=0;
	}else if(curmode==2){
		if(curframe>=normalanimation+fireanimation){
			if(automatic&&fired){
				curframe=normalanimation;
			}else{
				curmode=1;
				curframe=0;
			}
		}
	}else if(curmode==3){
		if(curframe>=normalanimation+fireanimation+reloadanimation-1){
			curmode=1;
			curframe=0;
			reloading=false;
		}
	}
	
	vector3d tmpVec(expected_pos-curpos);
	tmpVec.normalize();
	tmpVec*=changeSpeed;
	curpos+=tmpVec;
	if((std::abs(expected_pos.getX()-curpos.getX())<changeSpeed)&&(std::abs(expected_pos.getY()-curpos.getY())<changeSpeed)&&(std::abs(expected_pos.getZ()-curpos.getZ())<changeSpeed))curpos=expected_pos;
	
	tmpVec.change(expected_rot-currot);
	tmpVec.normalize();
	tmpVec*=changeSpeed;
	currot+=tmpVec;
	if((std::abs(expected_rot.getX()-currot.getX())<changeSpeed)&&(std::abs(expected_rot.getY()-currot.getY())<changeSpeed)&&(std::abs(expected_rot.getZ()-currot.getZ())<changeSpeed))currot=expected_rot;
	particles->update();
	
}

void weapon::show(){
	glPushMatrix();//Hacemos push y pop para poder manipular la camara y colocar el arma frente a la cámara
		glTranslatef(curpos.getX(),curpos.getY(),curpos.getZ());//Trasladamos hasta la posición actual
		glRotatef(currot.getX(),1,0,0);//Rotamos 
		glRotatef(currot.getY(),0,1,0);
		glRotatef(currot.getZ(),0,0,1);
		glCallList(frames[curframe]);//Llamamos a la animación actual
		for(int i=0;i<attachments.size();i++){
			glPushMatrix();
				glTranslatef(attachments[i]->position.getX(),attachments[i]->position.getY(),attachments[i]->position.getZ());
				glRotatef(attachments[i]->rotation.getX(),1,0,0);
				glRotatef(attachments[i]->rotation.getY(),0,1,0);
				glRotatef(attachments[i]->rotation.getZ(),0,0,1);
				glScalef(attachments[i]->scale.getX(),attachments[i]->scale.getY(),attachments[i]->scale.getZ());
				glCallList(attachments[i]->mesh);
			glPopMatrix();
		}
	glPopMatrix();
}

bool weapon::fire(vector3d& direction,vector3d camdirection){
	if(reloading)return false;
	if((!automatic&&!fired)||automatic){
		if(lastshot>=speed){
			if(bullets>0){
				if(aiming){//Si estamos apuntando la ajustamos la precision a la de apuntado, probablemente será necesario incluir retroceso segun el número de disparos
					direction.setX(camdirection.getX()+((float)((rand()%3-1)/aimprecision)));
					direction.setY(camdirection.getY()+((float)((rand()%3-1)/aimprecision)));
					direction.setZ(camdirection.getZ()+((float)((rand()%3-1)/aimprecision)));
				}else{//Si no apuntamos empleamos la precisión normal, de nuevo sería recomendable incluir retroceso
					direction.setX(camdirection.getX()+((float)((rand()%3-1)/precision)));
					direction.setY(camdirection.getY()+((float)((rand()%3-1)/precision)));
					direction.setZ(camdirection.getZ()+((float)((rand()%3-1)/precision)));
				}
				fired=true;
				lastshot=0;
				bullets--;
				particles->setNumParticles(4000);
				//std::cout<<"bullets:"<<bullets<<std::endl;
				curframe=normalanimation;
				curmode=2;
				//Aqui deberíamos aumentar el numero de balas disparadas para calcular el retroceso
				return true;
			}else{
				reload();
				return false;
			}
			
		}
	}
	return false;
}

void weapon::stopfire(){
	fired=false;
	//Aqui deberíamos resetear la cuenta de disparos para el retroceso;
}

void weapon::reload(){
	if(!reloading&&maxBullets!=bullets){
		reloading=true;
		if(allBullets>maxBullets-bullets){
			allBullets-=maxBullets-bullets;
			bullets=maxBullets;
		}else{
			bullets=allBullets+bullets;
			allBullets=0;
		}
		curframe=normalanimation+fireanimation;
		curmode=3;
	}
}

void weapon::aim(){
	aiming=!aiming;
	if(aiming){
		expected_rot=aimrotation;
		expected_pos=aimposition;
	}else{
		expected_rot=rotation;
		expected_pos=position;
	}
}

void weapon::test(attachment* att){
	if(isTest){
		vector3d* targ;
		particles->setNumParticles(5000);
		if(att)targ=&att->position;
		else targ=&expected_pos;
		targ=&particlePosition;
		
		Uint8* keys=SDL_GetKeyState(NULL);
		if(keys[SDLK_j])expected_pos.setX(expected_pos.getX()-0.02);
		if(keys[SDLK_l])expected_pos.setX(expected_pos.getX()+0.02);
		if(keys[SDLK_k])expected_pos.setZ(expected_pos.getZ()-0.02);
		if(keys[SDLK_i])expected_pos.setZ(expected_pos.getZ()+0.02);
		if(keys[SDLK_n])expected_pos.setY(expected_pos.getY()-0.02);
		if(keys[SDLK_m])expected_pos.setY(expected_pos.getY()+0.02);
		if(keys[SDLK_u])expected_rot.setY(expected_rot.getY()-0.4);
		if(keys[SDLK_o])expected_rot.setY(expected_rot.getY()+0.4);
		
		std::cout<<"pos:"<<expected_pos<<"rot:"<<expected_rot<<std::endl;
		
		if(att)std::cout << "Attachment: " << (*targ) << currot << std::endl;
		else std::cout << "weapon: " << (*targ) << currot << std::endl;
	}
}

void weapon::addBullets(unsigned int num){
	allBullets+=num;
}

void weapon::setBullets(unsigned int num){
	allBullets=num;
}
	
int weapon::getDamage(){
	return damage;
}

int weapon::getAmmo(){
	return bullets;
}

int weapon::getTotalAmmo(){
	return allBullets;
}

std::string weapon::getName(){
	return name;
}

std::vector<unsigned int>& weapon::getAnimation(){
	return frames;
}

bool weapon::isAimed(){
	return aiming;
}

unsigned int weapon::getOuterView(){
	return outerview;
}

void weapon::addAttachment(int i,unsigned int m,const vector3d& p,const vector3d& r,const vector3d& s,const vector3d& nnp,const vector3d& nap,const vector3d& nnr,const vector3d& nar)
{
	//Nota: no tengo ni idea de como o para que se usa esto, lo he copiado y dejado aqui para un posible uso en el futuro
	attachments.push_back(new attachment(i,m,p,r,s));
	position+=nnp;
	rotation+=nnr;

	aimposition+=nnr;
//	aimrotation+=nar;
}

void weapon::showParticleSystem(camera& cam)
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(particlePosition.getX()+curpos.getX(),particlePosition.getY()+curpos.getY(),particlePosition.getZ()+curpos.getZ());
		particles->show(cam);
	glPopMatrix();
}
