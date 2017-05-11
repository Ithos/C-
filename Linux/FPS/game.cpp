#include "game.h"

game::game(){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);
	glClearColor(0.5,0.5,0.5,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,640.0/480.0,0.01,500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	std::vector<collisionPlane> mapcp;
	std::vector<vector3d> mapsp;
	mapsp.push_back(vector3d(0,0,0));
	unsigned int map=obj.loadObject("testlevel2.obj",&mapcp);
	unsigned int sk[6];//Las texturas a cargar deberian ser distintas para cada nivel (o igual no, pero deberian poder serlo)
	sk[0]=loadTexture("leftLarge.bmp",false);
	sk[1]=loadTexture("backLarge.bmp",false);
	sk[2]=loadTexture("rightLarge.bmp",false);
	sk[3]=loadTexture("frontLarge.bmp",false);
	sk[4]=loadTexture("upLarge.bmp",false);
	sk[5]=loadTexture("bottomLarge.bmp",false);
	levels.push_back(new level("lev",map,mapcp,mapsp,&sk[0]));
	
	ps=new particlesystem(1000,vector3d(0,0,0),0.001,0.1,0.1,5.0,vector3d(0,0,-20),vector3d(255,150,150),vector3d(255,255,255),30,160,3000,loadTexture("smoke.png"));
	sparkles=new particlesystem(30,vector3d(0,0,0),0.01,0.3,1.0,20.0,vector3d(0,0,0),vector3d(255,255,255),vector3d(255,255,255),300,1000,0,loadTexture("spark.png"));
	
	std::vector<unsigned int> anim;
	loadAnimation(anim,"TestWeapon/testWeapon",36);
	weapons.push_back(new weapon(anim,anim[0],1,15,19,vector3d(0.2,-0.4,-1.2),vector3d(0,3.00,0),vector3d(0,-0.26,-0.94),vector3d(0,-3.4,0),100,1000,10,15,300,20,"weapon1",true,vector3d(-0.0,0.24,-0.0),ps));
	
	anim.clear();
	loadAnimation(anim,"knife/knife",5);
	player1=new player("player1",collisionSphere(vector3d(0,10,0),3.0),0.5,0.2,0.2,weapons[0],new knife(anim,1000,30,30,vector3d(-0.5,-0.6,-1.3)),new grenade(obj.loadObject("grenade.obj"),0.1,60,10,loadTexture("spark.png"),loadTexture("smoke.png")));
	
	anim.clear();
	loadAnimation(anim,"TestEnemy/enemy",65);
	enemies.push_back(new enemy(anim,30,25,10,100,5,0.1,collisionSphere(vector3d(5,10,0),2.0)));
	otherPlayersAnim=anim;//De momento usaremos la misma animación para los enemigos y para los otros jugadores
	online=false;//De momento decidimos aquí si vamos a conectarnos al servidor o no, en posteriores versiones debería decidirse en una interfaz
	char ip[20];
	if(online){
		//de momento obtenemos la ip de la línea de comados, en versiones futuras debería obtenerse de la interfaz
		std::cin.getline(ip,20);
	}
	if(online)net=new network(ip);
	
	bulletImpactImg=loadTexture("bulletimpact.png");
}

game::~game(){
	for(int i=0;i<levels.size();i++)delete levels[i];
	for(int i=0;i<weapons.size();i++)delete weapons[i];
	for(int i=0;i<enemies.size();i++)delete enemies[i];
	for(int i=0;i<otherPlayers.size();i++)delete otherPlayers[i];
	if(online) delete net;
	glDeleteTextures(1,&bulletImpactImg);
	delete ps;
	delete sparkles;
	SDL_Quit();
}

void game::start(){
	bool run=true;
	bool b[4]={0,0,0,0};
	Uint32 start;
	SDL_Event event;
	
	bool shoting;
	vector3d shotDir;
	
	while(run){
		start=SDL_GetTicks();
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_MOUSEBUTTONDOWN){
				player1->cam.mouseIn(true);
				SDL_ShowCursor(SDL_DISABLE);
				if(event.button.button==SDL_BUTTON_LEFT){
					shoting=true;
				}
				if(event.button.button==SDL_BUTTON_RIGHT){
					player1->getEquippedWeapon()->aim();
				}
				if(event.button.button==SDL_BUTTON_WHEELUP){
					player1->changeWeaponUp();
				}
				if(event.button.button==SDL_BUTTON_WHEELDOWN){
					player1->changeWeaponDown();
				}
			}
			if(event.type==SDL_MOUSEBUTTONUP){
				shoting=false;
				player1->getEquippedWeapon()->stopfire();
			}
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_p){
					player1->cam.mouseIn(false);
					SDL_ShowCursor(SDL_ENABLE);
				}
				if(event.key.keysym.sym==SDLK_ESCAPE){
					run=false;
				}
				if(event.key.keysym.sym==SDLK_UP){
					b[0]=1;
				}
				if(event.key.keysym.sym==SDLK_DOWN){
					b[1]=1;
				}
				if(event.key.keysym.sym==SDLK_RIGHT){
					b[2]=1;
				}
				if(event.key.keysym.sym==SDLK_LEFT){
					b[3]=1;
				}
				if(event.key.keysym.sym==SDLK_0){
					player1->changeWeapon(0);
				}
				if(event.key.keysym.sym==SDLK_1){
					player1->changeWeapon(1);
				}
				if(event.key.keysym.sym==SDLK_2){
					player1->changeWeapon(2);
				}
				if(event.key.keysym.sym==SDLK_r){
					player1->getEquippedWeapon()->reload();
				}
				if(event.key.keysym.sym==SDLK_e){
					player1->setKnife(enemies);
				}
				if(event.key.keysym.sym==SDLK_q){
					player1->dropGrenade();//El modelo no se carga bien
				}
				if(event.key.keysym.sym==SDLK_SPACE){
					player1->setJump();
				}
				if(event.key.keysym.sym==SDLK_LSHIFT){
					player1->setSprint();
				}
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_UP){
					b[0]=0;
				}
				if(event.key.keysym.sym==SDLK_DOWN){
					b[1]=0;
				}
				if(event.key.keysym.sym==SDLK_RIGHT){
					b[2]=0;
				}
				if(event.key.keysym.sym==SDLK_LEFT){
					b[3]=0;
				}
				if(event.key.keysym.sym==SDLK_LSHIFT){
					player1->stopSprint();
				}
			}
		}
	
		if(shoting){
			if(player1->getEquippedWeapon()->fire(shotDir,player1->cam.getVector())){
				
				float mindist=10000;
				float curdist;
				vector3d curpoint;
				std::vector<collisionPlane>& collplane=levels[0]->getCollisionPlane();
				
				for(int i=0;i<collplane.size();i++)
				{
					if(collision::rayplane(collplane[i].getNormal().getX(),collplane[i].getNormal().getY(),collplane[i].getNormal().getZ(),player1->getCollisionSphere().getCenter().getX(),player1->getCollisionSphere().getCenter().getY(),player1->getCollisionSphere().getCenter().getZ(),shotDir.getX(),shotDir.getY(),shotDir.getZ(),collplane[i].getPoints()[0],collplane[i].getPoints()[1],collplane[i].getPoints()[2],collplane[i].getPoints()[3],&curdist,&curpoint))
					{
					//	std::cout << collplane[i].p[0] << collplane[i].p[1] << collplane[i].p[2] << collplane[i].p[3] << std::endl;
						if(curdist<mindist)mindist=curdist;
						vector3d v0=collplane[i].getPoints()[3]-collplane[i].getPoints()[2];
						v0.normalize();
						vector3d v1=collplane[i].getPoints()[2]-collplane[i].getPoints()[1];
						v1.normalize();
						vector3d v2=v1+v0;
						v2.normalize();
						v0/=5.0;
						v1/=5.0;
						v2/=5.0;
						bulletImpacts.push_back(collisionPlane(collplane[i].getNormal(),curpoint,curpoint+v1,curpoint+v2,curpoint+v0));
						sparklePos=curpoint;
						std::cout << "cp:" << curpoint << std::endl;
						sparkles->setNumParticles(30);
					//	std::cout << "added: " << v0 << v1 << v2 << v3 << std::endl;
					}
				}
				
				if(online){//Si estamos en línea comprobamos impactos contra otros jugadores
					for(int i=0;i<otherPlayers.size();i++){
						if(collision::raysphere(otherPlayers[i]->position.getX(),otherPlayers[i]->position.getY(),otherPlayers[i]->position.getZ(),shotDir.getX(),shotDir.getY(),shotDir.getZ(),player1->getCollisionSphere().getCenter().getX(),player1->getCollisionSphere().getCenter().getY(),player1->getCollisionSphere().getCenter().getZ(),otherPlayers[i]->cs.getR())){
							net->sendShot(player1,otherPlayers[i]->id);
						}
					}
				}
				for(int i=0;i<enemies.size();i++){//Habría que incluir una comprobación de la distancia de colisión para el caso de que haya múltiples enemigos 
					if(collision::raysphere(enemies[i]->getCollisionSphere()->getCenter().getX(),enemies[i]->getCollisionSphere()->getCenter().getY(),enemies[i]->getCollisionSphere()->getCenter().getZ(),shotDir.getX(),shotDir.getY(),shotDir.getZ(),player1->getCollisionSphere().getCenter().getX(),player1->getCollisionSphere().getCenter().getY(),player1->getCollisionSphere().getCenter().getZ(),enemies[i]->getCollisionSphere()->getR())){
						enemies[i]->decreaseHealth(player1->getEquippedWeapon()->getDamage());
						std::cout<<"Enemy hit"<<std::endl;
					}
				}
			}
		}
		if(player1->getHealth()<0){
			//run=false;//Si la vida cae por debajo de 0 de momento cerramos, posteriormente daremos un mensaje o algo
			player1->setHealth(1000);
			std::cout<<"Player dead.Reseting life"<<std::endl;
		}
		update();
		show();
		
		SDL_GL_SwapBuffers();//Esto es equivalente al flip de sdl mandamos un buffer a la pantalla y cargamos el otro
		
		if(1000/30>(SDL_GetTicks()-start)){
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
		}
		
	}
}

void game::update(){
	for(int i=0;i<levels.size();i++)levels[i]->update();
	
	
	for(int i=0;i<enemies.size();i++){
		//std::cout<<"num:"<<i<<" hp:"<<enemies[i]->getHealth()<< std::endl;
		if(enemies[i]->update(levels[0]->getCollisionPlane(),player1->getCollisionSphere().getCenter())){
			//Hacemos cosas cuando el enemigo muere
			items.add(vector3d(0,0,0),vector3d(1,1,1),*enemies[i]->getCollisionSphere(),0,weapons[0]->getOuterView());
		}
		if(enemies[i]->disappear()){//Si disappear da true borramos el enemigo correspondiente
			delete enemies[i];
			enemies.erase(enemies.begin()+i);
		}
	}
	
	for(int i=0;i<enemies.size();i++){//Hay que realizar comprobaciones adicionales antes de quitar vida al jugador
		if(enemies[i]->setAttack(player1->getCollisionSphere())){
			//El enemigo ataca
			player1->setHealth(player1->getHealth()-enemies[i]->getStrength());
			std::cout<<"player->life:"<<player1->getHealth()<<std::endl;
		}
	}
	
	int tmp=items.update(player1->getCollisionSphere());
	if(tmp==0){
		std::cout<<"Weapon obtained"<<std::endl;
	}
	if(online){
		net->send(player1);
		net->recv(otherPlayers,weapons,player1,otherPlayersAnim);
	}
	
	ps->update();
	sparkles->update();
	
	//respawn//
	//vector3d rsp=levels[0]->getRandomSpawnPoint();
	/*if(rand()%1000==0){//Generación aleatoria
		enemies.push_back(new enemy(otherPlayersAnim,30,25,10,100,5,0.1,collisionSphere(vector3d(5,10,0),2.0)));
	}*/
	
}

void game::show(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	vector3d newPos=player1->cam.control();
	player1->update(levels[0]->getCollisionPlane(),newPos,enemies);//No me gusta que el update este aquí, estudiaré si puede modificarse
	player1->show();
	levels[0]->getSkybox()->drawSkybox();//Deberiamos tener una variable que almacenase el nivel actual y usarla aqui
	player1->cam.updateCamera();
	player1->showKnife();//Pongo esto aqui para que el fondo no parpadée al usar el cuchillo
	levels[0]->show();//Deberíamos crear una variable que almacene el nivel en el que estamos
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,bulletImpactImg);
	
	for(int i=0;i<bulletImpacts.size();i++){//Representamos los impactos de bala en los muros
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
			glTexCoord2f(1,0);
			glVertex3f(bulletImpacts[i].getPoints()[0].getX(),bulletImpacts[i].getPoints()[0].getY(),bulletImpacts[i].getPoints()[0].getZ());
			glTexCoord2f(0,0);
			glVertex3f(bulletImpacts[i].getPoints()[1].getX(),bulletImpacts[i].getPoints()[1].getY(),bulletImpacts[i].getPoints()[1].getZ());
			glTexCoord2f(0,1);
			glVertex3f(bulletImpacts[i].getPoints()[2].getX(),bulletImpacts[i].getPoints()[2].getY(),bulletImpacts[i].getPoints()[2].getZ());
			glTexCoord2f(1,1);
			glVertex3f(bulletImpacts[i].getPoints()[3].getX(),bulletImpacts[i].getPoints()[3].getY(),bulletImpacts[i].getPoints()[3].getZ());
		glEnd();
	}
	for(int i=0;i<enemies.size();i++)enemies[i]->show();
	items.show();
	if(online){
		for(int i=0;i<otherPlayers.size();i++)otherPlayers[i]->show();
	}	
	std::vector<grenade*>* grenades=player1->getGrenades();
	for(int i=0;i<(*grenades).size();i++)(*grenades)[i]->show();
	
	glPushMatrix();//Representamos brillos
		glTranslatef(sparklePos.getX(),sparklePos.getY(),sparklePos.getZ());
		sparkles->show(player1->cam);
	glPopMatrix();
	
	for(int i=0;i<(*grenades).size();i++){
		glTranslatef((*grenades)[i]->getPosition().getX() ,(*grenades)[i]->getPosition().getY(),(*grenades)[i]->getPosition().getZ());
		(*grenades)[i]->showParticleSystem(player1->cam);
	}
	
	
	player1->getEquippedWeapon()->showParticleSystem(player1->cam); //Esto lo emplearemos cuando modifiquemos las armas
}

void game::loadAnimation(std::vector<unsigned int>& frames,std::string filename,unsigned int num,std::vector<std::vector<collisionPlane> >* collplanes){
	char tmp[200];
	for(int i=1;i<=num;i++){
		sprintf(tmp,"_%06d.obj",i);//En tmp almacenamos el formato del nombre del archivo
		std::string tmp2(filename+tmp);
		if(collplanes!=NULL){
			std::vector<collisionPlane> tmpplane;
			unsigned int id=obj.loadObject(tmp2,&tmpplane);
			frames.push_back(id);
			collplanes->push_back(tmpplane);
		}else{
			unsigned int id=obj.loadObject(tmp2);
			frames.push_back(id);
		}
	}
}

unsigned int game::loadTexture(const char* filename,bool generate){
	SDL_Surface* image=IMG_Load(filename);//Cargamos la imagen mediante SDL
	if(image==NULL){//Chequeamos posibles errores
		std::cout<<"no image loaded"<<std::endl;
		return -1;
	}
	//Especificamos el formato al que se convertirá la imagen mediante un pixel format (mirar los apartados en la documentación)
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	//Convertimos la imagen a un formato independiente del archivo
	SDL_Surface* img2=SDL_ConvertSurface(image,&form,SDL_SWSURFACE);
	if(img2==NULL){//Chequeamos posibles errores
		std::cout<<"error in image format conversion"<<std::endl;
		return -1;
	}
	unsigned int id;//Creamos el identificador de la textura
	glGenTextures(1,&id);//Esta funcion nos pide el numero de texturas y la direccion de memoria de los identificadores //El segundo parametro dberia ser la dir de mem a un array con los ids de todas las texturas, en nuestro caso solo necesitamos 1 id
	glBindTexture(GL_TEXTURE_2D,id);//Indicamos el tipo de textura y el identificador
	if(generate) gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,image->w,image->h,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);//Con esto generamos los mipmaps en tiempo de ejecucion, si se quiere mejorar hay que generarlos previamente y cargarlos de forma distinta, mirar documentación
	else glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->w,image->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);//Cargamos la textura dando diversos parametros y le pasamos el mapa de pixeles
	if(generate)glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//Esto es por si se carga un mipmap
	else glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//Ajustamos el filtro minimo
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Ajustamos el filtro nose si maximo o magnificante o como sea
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	SDL_FreeSurface(image);//Eliminamos el surface pq los datos ya se han copiado
	SDL_FreeSurface(img2);
	return id;//Devolvemos el id de la textura
}
