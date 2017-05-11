#include "network.h"

network::network(const char* c){
	if(SDLNet_Init()==-1)std::cout<<"SDLNet failed to start"<<std::endl;
	IPaddress ip;
	if(SDLNet_ResolveHost(&ip,c,1234)==-1)std::cout<<"Cannot resolve host"<<std::endl;
	connection=SDLNet_TCP_Open(&ip);
	if(connection==NULL)std::cout<<"Bad ip"<<std::endl;
	server=SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(server,connection);
}

network::~network(){
	SDLNet_TCP_Send(connection,"2 \n",4);
	SDLNet_TCP_Close(connection);
	SDLNet_FreeSocketSet(server);
	SDLNet_Quit();
}

void network::send(player* p){
	if(p->getReady()){
		vector3d vec=p->cam.getVector();
		//Enviamos  1 id curframe position rotation direction health weapon
		sprintf(tmp,"1 %d %d %f %f %f %f %f %f %f %f %f %d %d \n",p->getId(),p->getCurframe(),p->getCollisionSphere().getCenter().getX(),p->getCollisionSphere().getCenter().getY(),p->getCollisionSphere().getCenter().getZ(),p->getRotation().getX(),p->getRotation().getY(),p->getRotation().getZ(),vec.getX(),vec.getY(),vec.getZ(),p->getHealth(),p->getWeaponIndex());
		int size=0;
		int len=strlen(tmp)+1;
		while(size<len){//Debido a que el mensaje probablemente no se enviará completo obtenemos el número de carácteres que se enviaron y continuamos a partir de ahí
			size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
		}
	}
}

void network::sendShot(player* p,int c){
	if(p->getReady()){
		sprintf(tmp,"3 %d %d %d \n",c,p->getEquippedWeapon()->getDamage(),p->getId());
		int size=0;
		int len=strlen(tmp)+1;
		while(size<len){//Debido a que el mensaje probablemente no se enviará completo obtenemos el número de carácteres que se enviaron y continuamos a partir de ahí
			size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
		}
	}
}

void network::recv(std::vector<otherPlayer*> &otherPlayers,std::vector<weapon*> weapons,player* p,std::vector<unsigned int> f){
	while(SDLNet_CheckSockets(server,0)>0 && SDLNet_SocketReady(connection)){
		int offset=0;
		do{//Recibiendo mensajes tenemos el mismo problema que enviandolos, por lo que tenemos que leer hasta identificar un carácter \n y además matener el número de carácteres que hemos leído
			offset+=SDLNet_TCP_Recv(connection,tmp+offset,1400);
			if(offset<=0)return;
		}while(tmp[strlen(tmp)-1]!='\n');
		int type,id;
		sscanf(tmp,"%d %d",&type,&id);
		if(type==0){
			p->setId(id);
		}else if(type==1){
			int i;
			for(i=0;i<otherPlayers.size();i++){
				if(otherPlayers[i]->id==id)
				{
					int tmp2;
					float positionX,positionY,positionZ,rotationX,rotationY,rotationZ,ldX,ldY,ldZ;
					sscanf(tmp,"1 %d %d %f %f %f %f %f %f %f %f %f %d %d \n",&tmp2,&(otherPlayers[i]->curframe),&positionX,&positionY,&positionZ,&rotationX,&rotationY,&rotationZ,&ldX,&ldY,&ldZ,&(otherPlayers[i]->health),&tmp2);
					otherPlayers[i]->w=weapons[tmp2];
					otherPlayers[i]->position.setX(positionX);
					otherPlayers[i]->position.setY(positionY);
					otherPlayers[i]->position.setZ(positionZ);
					otherPlayers[i]->rotation.setX(rotationX);
					otherPlayers[i]->rotation.setY(rotationY);
					otherPlayers[i]->rotation.setZ(rotationZ);
					otherPlayers[i]->lookDirection.setX(ldX);
					otherPlayers[i]->lookDirection.setY(ldY);
					otherPlayers[i]->lookDirection.setZ(ldZ);
					break;
				}
			}
			if(i>=otherPlayers.size()){
				otherPlayers.push_back(new otherPlayer(f,id));//Si no tenemos la id añadimos un nuevo jugador
			}
		}else if(type==2){
			for(int i=0;i<otherPlayers.size();i++){
				if(otherPlayers[i]->id==id){
					otherPlayers.erase(otherPlayers.begin()+i);
				}
			}
		}else if(type==3){//Recibimos el mensaje de que nos han impactado
			int tmp1,tmp2,tmp3;
			sscanf(tmp,"3 %d %d %d",&tmp1,&tmp2,&tmp3);
			p->setHealth(p->getHealth()-tmp2);
		}
	}
}
