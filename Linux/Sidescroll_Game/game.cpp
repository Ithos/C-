#include "game.h"

SDL_Rect baseclass::coord;//Coordenadas del jugador

game::game(){

	//Definimos el constructor	

	SDL_Init(SDL_INIT_EVERYTHING);
	screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32,SDL_SWSURFACE);
	Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
	SDL_WM_SetCaption( "Game", "Game" );
	block=load_image("blocks.bmp");
	background=load_image("background2.jpeg");//Necesito backgrounds nuevos
	bul=load_image("fireball.bmp");
	ene=load_image("enemy.bmp");
	simbol=load_image("cross.bmp");
	obs=load_image("obstacle.bmp");
	can=load_image("cannonAnim.bmp");
	eBul=load_image("enemyBullet.bmp");
	turr=load_image("turret.bmp");
	sBul=load_image("smallBullet.bmp");
	min=load_image("mine.bmp");
	baseclass::coord.x=baseclass::coord.y=camera.x=camera.y=0;
	baseclass::coord.w=camera.w=SCREEN_WIDTH;
	baseclass::coord.h=camera.h=SCREEN_HEIGHT;
	mapHeight=0;
	for(int i=0;i<4;i++)direction[i]=0;
	ctr=0;
	first=true;
	run=true;
	lvClear=false;
	keyPressed=false;//Aqui almacenaremos si alguna tecla de dirección se ha pulsado
	pl=	load_image("Player.bmp");
	player1=new player(pl);
	playerVel=5.0;
	finish.x=finish.y=0;
	finish.w=finish.h=50;
	

}

game::~game(){

	SDL_FreeSurface(background);
	SDL_FreeSurface(block);//Este nombre puede no ser el definitivo
	SDL_FreeSurface(pl);
	SDL_FreeSurface(bul);//Aqui liberamos el surface de bul que no liberamos en la clase bullet
	SDL_FreeSurface(ene);
	SDL_FreeSurface(simbol);
	SDL_FreeSurface(obs);
	SDL_FreeSurface(can);
	SDL_FreeSurface(eBul);
	SDL_FreeSurface(min);
	for(int i=0;i<bullets.size();i++)delete bullets[i];//Borramos las balas de la memoria
	for(int i=0;i<enemies.size();i++)delete enemies[i];//Borramos los enemigos de la memoria
	for(int i=0;i<obstacles.size();i++)delete obstacles[i];//Borramos los obstaculos de memoria 
	for(int i=0;i<movCubes.size();i++)delete movCubes[i];//Borramos los cubos moviles
	for(int i=0;i<fallingCubes.size();i++)delete fallingCubes[i];//Borramos los cubos que se caen
	for(int i=0;i<iceCubes.size();i++)delete iceCubes[i];//Borramos los cubos de hielo
	for(int i=0;i<mossCubes.size();i++)delete mossCubes[i];//Borramos los cubos mohosos
	for(int i=0;i<cannons.size();i++)delete cannons[i];//Borramos los cañones
	for(int i=0;i<enemyBullets.size();i++)delete enemyBullets[i];//Borramos las balas de los cañones
	for(int i=0;i<turrets.size();i++)delete turrets[i];//Borramos las torretas
	for(int i=0;i<smallBullets.size();i++)delete smallBullets[i];//Borramos las balas de las torretas
	for(int i=0;i<mines.size();i++)delete mines[i];//Borramos las minas flotantes
	for(int i=0;i<rubberCubes.size();i++)delete rubberCubes[i];//Borramos los cubos de goma
	Mix_CloseAudio();
	SDL_Quit();

}

SDL_Surface* game::load_image(const char* filename){//Cargamos imagenes y eliminamos colorkey

	SDL_Surface* tmp=IMG_Load(filename);//Cargamos la imagen mediante SDL_image
	SDL_Surface* tmp2=SDL_DisplayFormat(tmp);//Convierte la imagen al formato de la pantalla
	SDL_SetColorKey(tmp2,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,77,77,77)); //Damos como setcolorkey el color predefinido, la comentaremos hasta comprobar las colisiones
	SDL_FreeSurface(tmp);
	return tmp2;
	
}

void game::load_map(const char* filename){//Mediante esta funcion cargamos el mapa de map.map
	int width=0,height=0,Xini=0,Yini=0,Xplay=0,Yplay=0;
	char current;//Para facilitar el desarrollo de mapas empleamos chars
	
	std::ifstream in(filename);//Abrimos el archivo
	
	if(!in.is_open()){//Chequeamos si se ha abierto el archivo y si no mandamos un mensaje de error
		std::cout<<"Error loading the file."<<std::endl;
		return;
	}
	
	in>>width;
	in>>height;//Leemos los dos primeros datos del archivo que son anchura y altura y los almacenamos
	
	mapHeight=((int)(height*TILE_SIZE/SCREEN_HEIGHT))*SCREEN_HEIGHT;//Aqui almacenamos la altura del mapa
	

	in>>Xini;//Almacenamos las coordenadas iniciales
	in>>Yini;
	
	baseclass::coord.x=camera.x=Xini*TILE_SIZE;//Situamos la camara y las coordenadas en las coordenadas iniciales
	baseclass::coord.y=camera.y=Yini*TILE_SIZE-TILE_SIZE/5;//Al subir la pantalla 1/5 hay que poner una fila mas de bloques
	
	in>>Xplay;//Establecemos las coordenadas iniciales del jugador 
	in>>Yplay;
	
	player1->setX(Xplay*TILE_SIZE);//NOTA:Estas coordenadas son relativas a la pantalla
	player1->setY(Yplay*TILE_SIZE-TILE_SIZE/5);
	
	player1->setHealth(10);//Reseteamos posición y salud del jugador, camara y coordenadas
	
	for(int dir=0;dir<4;dir++)direction[dir]=0;//Esto es para que no empiece moviendose en una direccion
	player1->stopJump(); //Esto es para evitar que si el jugador ha muerto durante un salto empiece saltando
	for(int e=0;e<enemies.size();e=0){//Borramos siempre el primer elemento del vector hasta que se vacía pd:no se pq no funciona con while
		delete enemies[e];//Antes de empezar a cargar nada borramos los enemigos 
		enemies.erase(enemies.begin()+e);
	}
	for(int e=0;e<cannons.size();e=0){//Borramos siempre el primer elemento del vector hasta que se vacía pd:no se pq no funciona con while
		delete cannons[e];//Antes de empezar a cargar nada borramos los cañones
		cannons.erase(cannons.begin()+e);
	}
	for(int e=0;e<enemyBullets.size();e=0){//Borramos siempre el primer elemento del vector hasta que se vacía pd:no se pq no funciona con while
		delete enemyBullets[e];//Antes de empezar a cargar nada borramos los disparos 
		enemyBullets.erase(enemyBullets.begin()+e);
	}
	for(int e=0;e<turrets.size();e=0){//Borramos siempre el primer elemento del vector hasta que se vacía pd:no se pq no funciona con while
		delete turrets[e];//Antes de empezar a cargar nada borramos las torretas
		turrets.erase(turrets.begin()+e);
	}
	for(int e=0;e<smallBullets.size();e=0){//Borramos siempre el primer elemento del vector hasta que se vacía pd:no se pq no funciona con while
		delete smallBullets[e];//Antes de empezar a cargar nada borramos los disparos de las torretas
		smallBullets.erase(smallBullets.begin()+e);
	}
	for(int e=0;e<mines.size();e=0){//Borramos siempre el primer elemento del vector hasta que se vacía pd:no se pq no funciona con while
		delete mines[e];//Antes de empezar a cargar nada borramos las minas
		mines.erase(mines.begin()+e);
	}
	for(int e=0;e<obstacles.size();e=0){//Antes de cargar nada borramos el vector de obstculos por si acaso
		delete obstacles[e];
		obstacles.erase(obstacles.begin()+e);
	}
	for(int e=0;e<movCubes.size();e=0){//Cuando cargamos el mapa borramos los cubos moviles
		delete movCubes[e];
		movCubes.erase(movCubes.begin()+e);
	}
	for(int e=0;e<fallingCubes.size();e=0){//Cuando cargamos el mapa borramos los cubos que se caen
		delete fallingCubes[e];
		fallingCubes.erase(fallingCubes.begin()+e);
	}
	for(int e=0;e<iceCubes.size();e=0){//Borramos cubos de hielo
		delete iceCubes[e];
		iceCubes.erase(iceCubes.begin()+e);
	}
	
	for(int e=0;e<mossCubes.size();e=0){//Borramos cubos mohosos
		delete mossCubes[e];
		mossCubes.erase(mossCubes.begin()+e);
	}
	
	for(int e=0;e<rubberCubes.size();e=0){//Borramos cubos mohosos
		delete rubberCubes[e];
		rubberCubes.erase(rubberCubes.begin()+e);
	}
	
	for(int g=0;g<bullets.size();g=0){//Chequeamos para todas las balas	
		delete bullets[g];//borramos de la memoria y del vector
		bullets.erase(bullets.begin()+g);
	}
	
	for(int i=0;i<height;i++){
		std::vector<int> vec;//Creamos un vector 
	
		for(int j=0;j<width;j++){
			if(in.eof()){//Chequeamos si hemos llegado al final del archivo y mandamos un mensaje de error
				std::cout<<"Error reading the file."<<std::endl;
				return;
			}
			
			in>>current;//Leemos el valor actual y lo almacenamos
			if(current=='r'){
				obstacles.push_back(new obstacle(obs,j*50-1,i*50+5,3));//Creamos un obstaculo(derecha)
				vec.push_back(0);//Y mandamos un hueco al mapa 
			}else if(current=='q'){
				obstacles.push_back(new obstacle(obs,j*50+10,i*50+5,2));//Creamos un obstaculo(izquierda)
				vec.push_back(0);//Y mandamos un hueco al mapa 
			}else if(current=='p'){
				obstacles.push_back(new obstacle(obs,j*50+5,i*50-2,1));//Creamos un obstaculo(abajo)
				vec.push_back(0);//Y mandamos un hueco al mapa 
			}else if(current=='o'){
				obstacles.push_back(new obstacle(obs,j*50+5,i*50+10,0));//Creamos un obstaculo (arriba)
				vec.push_back(0);//Y mandamos un hueco al mapa 
			}else if(current=='e'){
				enemies.push_back(new enemy(ene,j*50,i*50,1,0));//Creamos un enemigo
				vec.push_back(0);//Y metemos un 0 en el vector q es un espacio vacio en el mapa
			}else if(current=='c'){//Cañon en el suelo
				cannons.push_back(new cannon(can,j*50+9,i*50+18,false));
				vec.push_back(0);
			}else if(current=='u'){//Cañon en el techo
				cannons.push_back(new cannon(can,j*50+9,i*50-2,true));
				vec.push_back(0);
			}else if(current=='t'){//torreta
				turrets.push_back(new turret(turr,j*50+7,i*50+20));
				vec.push_back(0);
			}else if(current=='l'){
				mines.push_back(new mine(min,j*50,i*50));
				vec.push_back(0);
			}else{	
				if(current=='d'){//Bloque de meta
					finish.x=j*50;
					finish.y=i*50;
					vec.push_back(9);
				}
				if(current=='*')vec.push_back(0);
				if(current=='b')vec.push_back(1);
				if(current=='h'){//Bloque que se mueve en horizontal
					movCubes.push_back(new movingCube(block,j*50,i*50,5,0,4,1));//Deberia modificar cosas para poder decir en el mapa la distancia y la velocidad
					vec.push_back(0);
				}
				if(current=='v'){//Bloque que se mueve en vertical
					movCubes.push_back(new movingCube(block,j*50,i*50,0,5,4,0));//Deberia modificar cosas para poder decir en el mapa la distancia y la velocidad
					vec.push_back(0);
				}
				if(current=='f'){//Bloque que se cae
					fallingCubes.push_back(new fallingCube(block,j*50,i*50,6));
					vec.push_back(0);
				}
				if(current=='i'){//Bloque de hielo
					iceCubes.push_back(new iceCube(block,j*50,i*50));
					vec.push_back(2);//Si se cambia el checkeo abajo por !=0 aqui hay que cambiar los 2 por 1
				}	
				if(current=='m'){//Bloque mohoso
					mossCubes.push_back(new mossCube(block,j*50,i*50,false));
					vec.push_back(2);//Si se cambia el checkeo abajo por !=0 aqui hay que cambiar los 2 por 1
				}
				if(current=='n'){//Bloque mohoso rotado
					mossCubes.push_back(new mossCube(block,j*50,i*50,true));
					vec.push_back(2);//Si se cambia el checkeo abajo por !=0 aqui hay que cambiar los 2 por 1
				}
				if(current=='g'){//Bloque de goma
					rubberCubes.push_back(new rubberCube(block,j*50,i*50));
					vec.push_back(2);//Si se cambia el checkeo abajo por !=0 aqui hay que cambiar los 2 por 1
				}
			}		
		}
			map.push_back(vec);	//Almacenamos el vector
	}
		
	in.close(); //Cerramos el archivo

	
}

void game::showmap(){//Aqui mostramos un bloque u otro segun lo que tengamos almacenado en map
	
	int start=(baseclass::coord.x-(baseclass::coord.x%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Dividimos la pantalla en cuadros y recorremos solo los cuadros de la pantalla para optimizar
	int end=(baseclass::coord.x+baseclass::coord.w+(baseclass::TILE_SIZE-(baseclass::coord.x+baseclass::coord.w)%baseclass::TILE_SIZE))/baseclass::TILE_SIZE;//Todo este codigo es para asegurar que los numeros que obtenemos son enteros
	
	if(start<0)start=0;//Esto es para que no leamos fuera de los vectores
	if(end>map[0].size())end=map[0].size();
	
	for(int i=0;i<map.size();i++)
		for(int j=start;j<end;j++)
			if(map[i][j]==1||map[i][j]==9){//anterior !=0 pero lo he cambiado para no renderizar cosas encima de cosas
				
				SDL_Rect blockrect={(map[i][j]-1)*baseclass::TILE_SIZE,0,baseclass::TILE_SIZE,baseclass::TILE_SIZE};//Aqui creamos un rectangulo de 50x50 y lo movemos segun una linea 50 unidades cada vez para seleccionar el tipo de bloque
				//Habra que crear un archivo llamado blocks con el formato correcto o modificar este rect para que se adapte a nuestro archivo
				
				
				SDL_Rect destrect={j*baseclass::TILE_SIZE-baseclass::coord.x,i*baseclass::TILE_SIZE-baseclass::coord.y};//Creamos el rect de destino y lo referimos a las coordenadas de la pantalla
				//Solo adaptamos las coordenadas en x ya que la pantalla solo se mueve en horizontal
				
				SDL_BlitSurface(block,&blockrect,screen,&destrect);//Dibujamos en el destino desde el origen y lo mandamos a la pantalla
				
				
			}
	
	
	
}

void game::handleEvents(){//En esta funcion manejamos los eventos
	SDL_Event event;
	while(SDL_PollEvent(&event)){//manejamos el evento; si no ocurre evento SDL_PollEvent=0 si ocurre !=0
			if(event.type==SDL_QUIT)run=false;
			if(event.type==SDL_KEYDOWN){
				if(event.key.keysym.sym==SDLK_LCTRL)ctr=1;
				if(event.key.keysym.sym==SDLK_RIGHT){
					direction[0]=1;
					keyPressed=true;//si se pulsa una tecla de dirección almacenamos true
				}
				if(event.key.keysym.sym==SDLK_LEFT){
					direction[1]=1;
					keyPressed=true;
				}
				if(event.key.keysym.sym==SDLK_UP)direction[2]=1;
				if(event.key.keysym.sym==SDLK_DOWN)direction[3]=1;
				if(event.key.keysym.sym==SDLK_z)player1->setJump();
				if(event.key.keysym.sym==SDLK_ESCAPE){
					int j=menu().show(screen,"Continue");//Aqui esta el menu
					if(j==1)run=false;
					for(int dir=0;dir<4;dir++)direction[dir]=0;//Si abrimos el menu mientras pulsamos una direccion ponemos direction a 0
				}
				if(event.key.keysym.sym==SDLK_LSHIFT){//Aqui gestionamos los disparos
					if(bullets.size()<=2){
						if((player1->getDirection()=='r'||player1->getDirection()=='j')&&!direction[2]&&!direction[3])
							bullets.push_back(new bullet(bul,player1->getRect()->x+player1->getRect()->w-10,player1->getRect()->y+10,8,0));
						else if((player1->getDirection()=='r'||player1->getDirection()=='j')&&!direction[2]&&direction[3])
							bullets.push_back(new bullet(bul,player1->getRect()->x+player1->getRect()->w-10,player1->getRect()->y+10,8,8));
						else if((player1->getDirection()=='r'||player1->getDirection()=='j')&&direction[2]&&!direction[3]&&!ctr)
							bullets.push_back(new bullet(bul,player1->getRect()->x+player1->getRect()->w-10,player1->getRect()->y+10,8,-8));
						if((player1->getDirection()=='l'||player1->getDirection()=='k')&&!direction[2]&&!direction[3])
							bullets.push_back(new bullet(bul,player1->getRect()->x,player1->getRect()->y+10,-8,0));
						else if((player1->getDirection()=='l'||player1->getDirection()=='k')&&!direction[2]&&direction[3])
							bullets.push_back(new bullet(bul,player1->getRect()->x,player1->getRect()->y+10,-8,8));
						else if((player1->getDirection()=='l'||player1->getDirection()=='k')&&direction[2]&&!direction[3]&&!ctr)
							bullets.push_back(new bullet(bul,player1->getRect()->x,player1->getRect()->y+10,-8,-8));
						if(direction[2]&&!direction[0]&&!direction[1]&&ctr)
							bullets.push_back(new bullet(bul,player1->getRect()->x+player1->getRect()->w/2,player1->getRect()->y,0,-8));
						else if((player1->getDirection()=='r'||player1->getDirection()=='j')&&direction[2]&&!direction[3]&&ctr)
							bullets.push_back(new bullet(bul,player1->getRect()->x+player1->getRect()->w-10,player1->getRect()->y+10,8,-8));
						else if((player1->getDirection()=='l'||player1->getDirection()=='k')&&direction[2]&&!direction[3]&&ctr)
							bullets.push_back(new bullet(bul,player1->getRect()->x,player1->getRect()->y+10,-8,-8));
						}
				}
				//Incluimos un puntero a bullet en el vector
			}
			if(event.type==SDL_KEYUP){
				if(event.key.keysym.sym==SDLK_LCTRL)ctr=0;
				if(event.key.keysym.sym==SDLK_RIGHT){
					direction[0]=0;
					keyPressed=false;//Cuando se deja de pulsar almacenamos false
				}
				if(event.key.keysym.sym==SDLK_LEFT){
					direction[1]=0;
					keyPressed=false;
				}
				if(event.key.keysym.sym==SDLK_UP)direction[2]=0;
				if(event.key.keysym.sym==SDLK_DOWN)direction[3]=0;
				if(event.key.keysym.sym==SDLK_z)player1->stopJump();
			}
	}

}

bool game::start(){//iniciamos el juego
	Uint32 start; //Integer 32 bits	
	
	
	if(first){
		int j=menu().show(screen,"Start");//Aqui esta el menu inicial
		if(j==1)run=false;
		
	}else{
		if(lvClear){
			menu* menu1=new menu();
			menu1->showmessage(screen,"LEVEL CLEAR",0,255,0);
			int j=menu1->show(screen,"Retry");//Aqui esta el menu de retry
			if(j==1)run=false;
			lvClear=false;
			delete menu1;
		}else{
			menu* menu1=new menu();
			menu1->showmessage(screen,"YOU DIED",255,0,0);
			int j=menu1->show(screen,"Retry");//Aqui esta el menu de retry
			if(j==1)run=false;
			delete menu1;
		}
		
	}
	
	load_map("map_newFormat.map");
	
	while(run){
		start=SDL_GetTicks();//cogemos el tiempo de ejecucion del programa
		handleEvents();
		
		//Aqui gestionamos el movimiento del jugador y la camara//
		if(!ctr){
			if(direction[1]){
				keyPressed=true;
				if(!player1->getOnIce())playerVel=-5.0;
				player1->setMoving(1);
				player1->setDirection('l');
				if(player1->getRect()->x>150){
					player1->setXvel(playerVel);	
					player1->setXvel_Abs(playerVel);			
				}else{
					player1->setXvel(0);
					player1->setXvel_Abs(playerVel);
					camera.x+=playerVel;
					baseclass::coord.x+=playerVel;
				}
				if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
				
			}else if(direction[0]){
				keyPressed=true;
				if(!player1->getOnIce())playerVel=5.0;
				player1->setMoving(1);
				player1->setDirection('r');
				if(player1->getRect()->x<150){
					player1->setXvel(playerVel);
					player1->setXvel_Abs(playerVel);				
				}else{
					player1->setXvel(0);
					player1->setXvel_Abs(playerVel);
					camera.x+=playerVel;
					baseclass::coord.x+=playerVel;
				}
				if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
				
			}else{
				player1->setMoving(0);
				player1->setXvel(0);
				player1->setXvel_Abs(0);
			}
		}else{
			if(!player1->getOnIce()){
				player1->setXvel(0);
				player1->setXvel_Abs(0);
				playerVel=0.0;
			}
			player1->setMoving(0);
			keyPressed=false;
			if(direction[1]){
				player1->setDirection('l');
			}
			if(direction[0]){
				player1->setDirection('r');
			}
		}
		
		//Aqui tenemos el movimiento vertical de la pantalla
		
		
		if(player1->getRect()->y>=SCREEN_HEIGHT&&baseclass::coord.y+SCREEN_HEIGHT<=mapHeight){
			camera.y+=SCREEN_HEIGHT;
			baseclass::coord.y+=SCREEN_HEIGHT;
			player1->setY(player1->getRect()->y-SCREEN_HEIGHT);
			//Si cambiamos de pantalla borramos las balas
			for(int g=0;g<bullets.size();g=0){//Chequeamos para todas las balas	
				delete bullets[g];//borramos de la memoria y del vector
				bullets.erase(bullets.begin()+g);
			}
		}
		
		if(player1->getRect()->y<=0&&baseclass::coord.y-SCREEN_HEIGHT>=0){
			camera.y-=SCREEN_HEIGHT;
			baseclass::coord.y-=SCREEN_HEIGHT;
			player1->setY(player1->getRect()->y+SCREEN_HEIGHT);
			for(int g=0;g<bullets.size();g=0){//Chequeamos para todas las balas	
				delete bullets[g];//borramos de la memoria y del vector
				bullets.erase(bullets.begin()+g);
			}
		}//Conviene mejorar esto, mejor un seguimiento continuo(quiza se pueda hacer un apaño si se ponen bloques mas pequeños)
		//Al mover la pantalla un poco para arriba se ha solucionado, aun asi podria estar bien implementarlo
		
		
		//Colisiones de balas con el mapa
		for(int g=0;g<bullets.size();g++){
			if(bullets[g]->bulletCollision(map)){//Cada bala dice si se ha chocado		
				delete bullets[g];//borramos de la memoria y del vector
				bullets.erase(bullets.begin()+g);
						
			}
		}
		
		for(int i=0;i<enemyBullets.size();i++){
			//Colisiones con el mapa
			if(enemyBullets[i]->bulletCollision(map)){//Cada bala dice si se ha chocado	
				delete enemyBullets[i];//borramos de la memoria y del vector
				enemyBullets.erase(enemyBullets.begin()+i);
						
			}
		}
		
		for(int i=0;i<smallBullets.size();i++){
			//Colisiones con el mapa
			if(smallBullets[i]->bulletCollision(map)){//Cada bala dice si se ha chocado	
				delete smallBullets[i];//borramos de la memoria y del vector
				smallBullets.erase(smallBullets.begin()+i);
						
			}
		}
		
		
		
		//Aqui chequeamos colisiones entre balas y enemigos
		for(int g=0;g<bullets.size();g++){
			
			for(int e=0;e<enemies.size();e++){
				if(!enemies[e]->getStomp()&&!enemies[e]->getHit()&&enemies[e]->getRect()->x+enemies[e]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&enemies[e]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&enemies[e]->getRect()->y+enemies[e]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&enemies[e]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos con enemigos dentro de la pantalla 
					SDL_Rect tmprect={enemies[e]->getRect()->x-baseclass::coord.x,enemies[e]->getRect()->y-baseclass::coord.y,30,30};
					if(collision(&tmprect,bullets[g]->getRect())){
						delete bullets[g];//borramos de la memoria y del vector la bala
						enemies[e]->setHit();
						bullets.erase(bullets.begin()+g);//el +e es para elegir el numero de enemigo/bala a borrar
						//Deberiamos implementar y mostrar la salud de los enemigos
					}
				}else if(enemies[e]->getDeletion()){ //Esperamos a borrar para poder mostrar una pequeña animación 
					delete enemies[e];
					enemies.erase(enemies.begin()+e);//el +e es para elegir el numero de enemigo/bala a borrar
				}
			}
		}
		
		//Colisiones con cañones
		for(int g=0;g<bullets.size();g++){
			for(int e=0;e<cannons.size();e++){
				if(cannons[e]->getRect()->x+cannons[e]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&cannons[e]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&cannons[e]->getRect()->y+cannons[e]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&cannons[e]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Chequeamos colisiones solo con los que esten en pantalla
				SDL_Rect tmprect={cannons[e]->getRect()->x-baseclass::coord.x,cannons[e]->getRect()->y-baseclass::coord.y,32,32};//Rectangulo en coordenadas relativas
					if(collision(&tmprect,bullets[g]->getRect())){
						delete bullets[g];//borramos de la memoria y del vector la bala
						if(cannons[e]->hit()){
							delete cannons[e];
							cannons.erase(cannons.begin()+e);
						}
						bullets.erase(bullets.begin()+g);//el +e es para elegir el numero de enemigo/bala a borrar
					}
				}
			}
			for(int e=0;e<turrets.size();e++){//Colisiones con torretas
				if(turrets[e]->getRect()->x+turrets[e]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&turrets[e]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&turrets[e]->getRect()->y+turrets[e]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&turrets[e]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Chequeamos colisiones solo con los que esten en pantalla
				SDL_Rect tmprect={turrets[e]->getRect()->x-baseclass::coord.x,turrets[e]->getRect()->y-baseclass::coord.y,35,30};//Rectangulo en coordenadas relativas
					if(collision(&tmprect,bullets[g]->getRect())){
						delete bullets[g];//borramos de la memoria y del vector la bala
						if(turrets[e]->hit()){
							delete turrets[e];
							turrets.erase(turrets.begin()+e);
						}
						bullets.erase(bullets.begin()+g);//el +e es para elegir el numero de enemigo/bala a borrar
					}
				}
			}
		}
		
		
		
		for(int i=0;i<enemyBullets.size();i++){
			//Colisiones con jugadores
			if(enemyBullets[i]->getRect()->x+enemyBullets[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&enemyBullets[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&enemyBullets[i]->getRect()->y+enemyBullets[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&enemyBullets[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){
				SDL_Rect tmprect={enemyBullets[i]->getRect()->x-baseclass::coord.x,enemyBullets[i]->getRect()->y-baseclass::coord.y,20,20};
				
				if(collision(&tmprect,player1->getRect())){
					player1->setHealth(player1->getHealth()-3);
					
					
					if(enemyBullets[i]->getXvel()>0){
						player1->setXvel(10);
						player1->setXvel_Abs(10);
						player1->move(map);
						camera.x+=10;
						baseclass::coord.x+=10;
						if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
					}else{
						player1->setXvel(-10);
						player1->setXvel_Abs(-10);
						player1->move(map);
						camera.x-=10;
						baseclass::coord.x-=10;
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
						
						player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
						player1->setXvel_Abs(-1);
						player1->move(map);
					}
					
					delete enemyBullets[i];//borramos de la memoria y del vector
					enemyBullets.erase(enemyBullets.begin()+i);
					
				}
				
			}
			
		}
		
		//Colisiones de balas de torreta con jugadores
		for(int i=0;i<smallBullets.size();i++){
			//Colisiones con jugadores
			if(smallBullets[i]->getRect()->x+smallBullets[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&smallBullets[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&smallBullets[i]->getRect()->y+smallBullets[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&smallBullets[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){
				SDL_Rect tmprect={smallBullets[i]->getRect()->x-baseclass::coord.x,smallBullets[i]->getRect()->y-baseclass::coord.y,20,20};
				
				if(collision(&tmprect,player1->getRect())){
					player1->setHealth(player1->getHealth()-1);
					
					
					if(smallBullets[i]->getXvel()>0){
						player1->setXvel(10);
						player1->setXvel_Abs(10);
						player1->move(map);
						camera.x+=10;
						baseclass::coord.x+=10;
						if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
					}else{
						player1->setXvel(-10);
						player1->setXvel_Abs(-10);
						player1->move(map);
						camera.x-=10;
						baseclass::coord.x-=10;
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
						
						player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
						player1->setXvel_Abs(-1);
						player1->move(map);
					}
					
					delete smallBullets[i];//borramos de la memoria y del vector
					smallBullets.erase(smallBullets.begin()+i);
					
				}
				
			}
			
		}
		//Colisiones entre minas y jugadores
		for(int i=0;i<mines.size();i++){
			if(mines[i]->getRect()->x+mines[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&mines[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&mines[i]->getRect()->y+mines[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&mines[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){
				SDL_Rect tmprect={mines[i]->getRect()->x-baseclass::coord.x,mines[i]->getRect()->y-baseclass::coord.y,mines[i]->getRect()->w,mines[i]->getRect()->h};
				if(collision(player1->getRect(),&tmprect)){
					mines[i]->hit();
					player1->setHealth(player1->getHealth()-4);
					if(player1->getRect()->x+player1->getRect()->w/2>=tmprect.x+tmprect.w/2){
						player1->setXvel((tmprect.w/2)+1);
						player1->setXvel_Abs((tmprect.w/2)+1);
						player1->move(map);
						camera.x+=((tmprect.w/2)+1);
						baseclass::coord.x+=((tmprect.w/2)+1);
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
					}else{
						player1->setXvel(-((tmprect.w/2)+1));
						player1->setXvel_Abs(-((tmprect.w/2)+1));
						camera.x-=((tmprect.w/2)+1);
						player1->move(map);
						baseclass::coord.x-=((tmprect.w/2)+1);
						if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
						
					}
					if(player1->getRect()->y+player1->getRect()->h/2>=tmprect.y+tmprect.h/2){
						player1->setYvel(((tmprect.h/2)+1));
						player1->setYvel_Abs((tmprect.h/2)+1);
						player1->move(map);
					}else{
						player1->setYvel(-((tmprect.h/2)+1));
						player1->setYvel_Abs(-((tmprect.h/2)+1));
						player1->move(map);
					}
				}
			}
		}
		
		
		//Nota:Para modificar esto y pasarlo a colisiones binarias habria que meterlo en la funcion move de player 
		//Aqui van las colisiones entre enemigos y jugadores
		for(int e=0;e<enemies.size();e++){
			if(!enemies[e]->getStomp()&&!enemies[e]->getHit()&&enemies[e]->getRect()->x+enemies[e]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&enemies[e]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&enemies[e]->getRect()->y+enemies[e]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&enemies[e]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos con enemigos dentro de la pantalla 
				SDL_Rect tmprect={enemies[e]->getRect()->x-baseclass::coord.x,enemies[e]->getRect()->y-baseclass::coord.y,30,30};
				
				if(collision(&tmprect,player1->getRect())){
					if(player1->getRect()->y+player1->getRect()->h>=tmprect.y&&player1->getRect()->y+player1->getRect()->h<=tmprect.y+14){//Chequeamos si estamos sobre el enemigo
						enemies[e]->setStomp();
						if(!player1->getGround())player1->setGroundTrue();//si ground es false lo cambiamos a true
						player1->setJump();
						
					}else{
						if(player1->getRect()->x+player1->getRect()->w>=tmprect.x&&player1->getRect()->x+player1->getRect()->w<=tmprect.x+12){//Chequeamos colisión por la derecha
							
							player1->setHealth(player1->getHealth()-1);//Deberiamos mostrar la salud del jugador
							player1->setXvel(-10);
							player1->setXvel_Abs(-10);
							player1->move(map);
							camera.x-=10;
							baseclass::coord.x-=10;
							if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
							
							enemies[e]->enemyCollision();
							player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
							player1->setXvel_Abs(-1);
							player1->move(map);
													
						}else{
							if(player1->getRect()->x<=tmprect.x+tmprect.w&&player1->getRect()->x>=tmprect.x+tmprect.w-12){//Chequeamos colisión por la izquierda
						
								player1->setHealth(player1->getHealth()-1);
								player1->setXvel(10);
								player1->setXvel_Abs(10);
								player1->move(map);
								camera.x+=10;
								baseclass::coord.x+=10;
								if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
								
								enemies[e]->enemyCollision();
							
							}else{
								if(player1->getRect()->y>=tmprect.y+tmprect.h-15&&player1->getRect()->y<=tmprect.y+tmprect.h){//Chequeamos colision por abajo
									player1->setHealth(player1->getHealth()-1);
									player1->setY(player1->getRect()->y+10);
									player1->setYvel(0);
									player1->setYvel_Abs(0);
									//De momento parece que funciona pero cuidado con esto
								}
								
							}
						
						}
					
					}
					
					
				}
			}else if(enemies[e]->getDeletion()){//Esperamos a borrar para poder mostrar una pequeña animación  //Este borrado puede que sobre, pero no consume mucho y sirve para asegurarse de hacer las cosas bien
				delete enemies[e];
				enemies.erase(enemies.begin()+e);
			}	
		}
		//Colisiones entre jugadores y obstaculos
		for(int i=0;i<obstacles.size();i++){
			if(obstacles[i]->getRect()->x+obstacles[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&obstacles[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&obstacles[i]->getRect()->y+obstacles[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&obstacles[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos con obstaculos dentro de la pantalla 
				SDL_Rect tmprect={obstacles[i]->getRect()->x-baseclass::coord.x,obstacles[i]->getRect()->y-baseclass::coord.y,40,40};//Rectangulo en coordenadas relativas
				
				if(collision(&tmprect,player1->getRect())){
					if(obstacles[i]->getRot()==0){
						if(player1->getRect()->x+player1->getRect()->w<tmprect.x+tmprect.w/2){
							if(2*(player1->getRect()->x+player1->getRect()->w-tmprect.x)>=-(player1->getRect()->y+player1->getRect()->h-tmprect.y-tmprect.h)){//Pasamos a coordenadas del obstaculo y comprobamos si pasa del borde del triangulo
								player1->setHealth(player1->getHealth()-1);//Deberiamos mostrar la salud del jugador
								player1->setXvel(-10);
								player1->setXvel_Abs(-10);
								player1->setYvel(-10);
								player1->setYvel_Abs(-10);
								player1->move(map);
								camera.x-=10;
								baseclass::coord.x-=10;
								if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
								
								player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
								player1->setXvel_Abs(-1);
								player1->move(map);
							}
						}else if(2*(player1->getRect()->x-tmprect.x-tmprect.w)<=player1->getRect()->y+player1->getRect()->h-tmprect.y-tmprect.h){//Pasamos a coordenadas en el otro extremo del obstaculo y comprobamos si el jugador toca el triangulo
								player1->setHealth(player1->getHealth()-1);
								player1->setXvel(10);
								player1->setXvel_Abs(10);
								player1->setYvel(-10);
								player1->setYvel_Abs(-10);
								player1->move(map);
								camera.x+=10;
								baseclass::coord.x+=10;
								if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
						}
					}else if(obstacles[i]->getRot()==1){
						if(player1->getRect()->x+player1->getRect()->w<tmprect.x+tmprect.w/2){
							if(2*(player1->getRect()->x+player1->getRect()->w-tmprect.x-tmprect.w/2)>=player1->getRect()->y-tmprect.y-tmprect.h){//En este caso el origen de coordenadas esta en el vertice del triangulo
								player1->setHealth(player1->getHealth()-1);
								player1->setXvel(-10);
								player1->setXvel_Abs(-10);
								player1->setYvel(10);
								player1->setYvel_Abs(10);
								player1->move(map);
								camera.x-=10;
								baseclass::coord.x-=10;
								if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
								
								player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
								player1->setXvel_Abs(-1);
								player1->move(map);	
								
							}
							
							
						}else if(2*(player1->getRect()->x-tmprect.x-tmprect.w/2)<=-(player1->getRect()->y-tmprect.y-tmprect.h)){
							player1->setHealth(player1->getHealth()-1);
							player1->setXvel(10);
							player1->setXvel_Abs(10);
							player1->setYvel(10);
							player1->setYvel_Abs(10);
							player1->move(map);
							camera.x+=10;
							baseclass::coord.x+=10;
							if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
							
						}
						
					}else if(obstacles[i]->getRot()==2){
						if(((player1->getRect()->x+player1->getRect()->w-tmprect.x)<2*(player1->getRect()->y-tmprect.y-tmprect.h/2))||((player1->getRect()->x+player1->getRect()->w-tmprect.x)<-2*(player1->getRect()->y+player1->getRect()->h-tmprect.y-tmprect.h/2))){
							//En este caso el orgen de coordenadas esta en el vertice del triangulo, chequeamos las condiciones en las que podemos colisionar sin tocar el triangulo y si no se dan movemos
						}else{
							player1->setHealth(player1->getHealth()-1);
							player1->setXvel(-10);
							player1->setXvel_Abs(-10);
							player1->setYvel(10);
							player1->setYvel_Abs(10);
							player1->move(map);
							camera.x-=10;
							baseclass::coord.x-=10;
							if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
								
							player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
							player1->setXvel_Abs(-1);
							player1->move(map);	
						}
					}else if(obstacles[i]->getRot()==3){
						if(((player1->getRect()->x-tmprect.x-tmprect.w)>2*(player1->getRect()->y+player1->getRect()->h-tmprect.y-tmprect.h/2))||(-(player1->getRect()->x-tmprect.x-tmprect.w)<2*(player1->getRect()->y-tmprect.y-tmprect.h/2))){
							//De nuevo ponemos el origen de coordenadas en el vertice y chequeamos las posiciones en las que el jugador no colisiona
						}else{
							player1->setHealth(player1->getHealth()-1);
							player1->setXvel(10);
							player1->setXvel_Abs(10);
							player1->setYvel(10);
							player1->setYvel_Abs(10);
							player1->move(map);
							camera.x+=10;
							baseclass::coord.x+=10;
							if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
						}
					}
					
				}
			}
		}
		
		for(int i=0;i<cannons.size();i++){//Colisiones entre jugadores y cañones
			if(cannons[i]->getRect()->x+cannons[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&cannons[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&cannons[i]->getRect()->y+cannons[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&cannons[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Chequeamos colisiones solo con los que esten en pantalla
				SDL_Rect tmprect={cannons[i]->getRect()->x-baseclass::coord.x,cannons[i]->getRect()->y-baseclass::coord.y,32,32};//Rectangulo en coordenadas relativas
				
				
				if(cannons[i]->getHit())cannons[i]->hitCount();
				if(cannons[i]->count()){
					if(cannons[i]->getUpDown()){
						enemyBullets.push_back(new enemyBullet(eBul,cannons[i]->getRect()->x+cannons[i]->getRect()->w/2-10,cannons[i]->getRect()->y+cannons[i]->getRect()->h+3,0,3));
					}else{
						enemyBullets.push_back(new enemyBullet(eBul,cannons[i]->getRect()->x+cannons[i]->getRect()->w/2-10,cannons[i]->getRect()->y-cannons[i]->getRect()->h-3,0,-3));
					}
				}
				
				if(collision(&tmprect,player1->getRect())){
					if(player1->getRect()->y+player1->getRect()->h>=tmprect.y&&player1->getRect()->y+player1->getRect()->h<=tmprect.y+14){//Colision por arriba
					
						player1->setHealth(player1->getHealth()-1);//Deberiamos mostrar la salud del jugador
						player1->setXvel(-10);
						player1->setXvel_Abs(-10);
						player1->setYvel(-10);
						player1->setYvel_Abs(-10);
						player1->move(map);
						camera.x-=10;
						baseclass::coord.x-=10;
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
								
						player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
						player1->setXvel_Abs(-1);
						player1->move(map);
						
					}else if(player1->getRect()->x+player1->getRect()->w>=tmprect.x&&player1->getRect()->x+player1->getRect()->w<=tmprect.x+12){//Chequeamos colisión por la derecha
					
						player1->setHealth(player1->getHealth()-1);//Deberiamos mostrar la salud del jugador
						player1->setXvel(-10);
						player1->setXvel_Abs(-10);
						player1->move(map);
						camera.x-=10;
						baseclass::coord.x-=10;
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
							
						player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
						player1->setXvel_Abs(-1);
						player1->move(map);
					
					}else if(player1->getRect()->x<=tmprect.x+tmprect.w&&player1->getRect()->x>=tmprect.x+tmprect.w-12){//Chequeamos colisión por la izquierda
					
						player1->setHealth(player1->getHealth()-1);
							player1->setXvel(10);
							player1->setXvel_Abs(10);
							player1->move(map);
							camera.x+=10;
							baseclass::coord.x+=10;
							if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
							
					}else if(player1->getRect()->y>=tmprect.y+tmprect.h-15&&player1->getRect()->y<=tmprect.y+tmprect.h){//Chequeamos colision por abajo
						player1->setHealth(player1->getHealth()-1);
						player1->setY(player1->getRect()->y+10);
						player1->setYvel(0);
						player1->setYvel_Abs(0);
					}
				}
				
			}
		}
		
		for(int i=0;i<turrets.size();i++){//Colisiones entre jugadores y torretas
			if(turrets[i]->getRect()->x+turrets[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&turrets[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&turrets[i]->getRect()->y+turrets[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&turrets[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Chequeamos colisiones solo con los que esten en pantalla
				SDL_Rect tmprect={turrets[i]->getRect()->x-baseclass::coord.x,turrets[i]->getRect()->y-baseclass::coord.y,32,32};//Rectangulo en coordenadas relativas
				
				
				if(turrets[i]->getHit())turrets[i]->hitCount(player1->getRect()->x);
				char c=turrets[i]->count(player1->getRect()->x);
				if(c!='m'){
					//incluimos balas en el vector
					if(c=='l')smallBullets.push_back(new smallBullet(sBul,turrets[i]->getRect()->x,turrets[i]->getRect()->y+2,-10,0,c));
					if(c=='r')smallBullets.push_back(new smallBullet(sBul,turrets[i]->getRect()->x+turrets[i]->getRect()->w,turrets[i]->getRect()->y+2,10,0,c));
				}
				
				if(collision(&tmprect,player1->getRect())){
					if(player1->getRect()->y+player1->getRect()->h>=tmprect.y&&player1->getRect()->y+player1->getRect()->h<=tmprect.y+14){//Colision por arriba
					
						player1->setHealth(player1->getHealth()-1);//Deberiamos mostrar la salud del jugador
						player1->setXvel(-10);
						player1->setXvel_Abs(-10);
						player1->setYvel(-10);
						player1->setYvel_Abs(-10);
						player1->move(map);
						camera.x-=10;
						baseclass::coord.x-=10;
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
								
						player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
						player1->setXvel_Abs(-1);
						player1->move(map);
						
					}else if(player1->getRect()->x+player1->getRect()->w>=tmprect.x&&player1->getRect()->x+player1->getRect()->w<=tmprect.x+12){//Chequeamos colisión por la derecha
					
						player1->setHealth(player1->getHealth()-1);//Deberiamos mostrar la salud del jugador
						player1->setXvel(-10);
						player1->setXvel_Abs(-10);
						player1->move(map);
						camera.x-=10;
						baseclass::coord.x-=10;
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
							
						player1->setXvel(-1);//Esto es una solucion un poco mala para un problema de tunneling
						player1->setXvel_Abs(-1);
						player1->move(map);
					
					}else if(player1->getRect()->x<=tmprect.x+tmprect.w&&player1->getRect()->x>=tmprect.x+tmprect.w-12){//Chequeamos colisión por la izquierda
					
						player1->setHealth(player1->getHealth()-1);
							player1->setXvel(10);
							player1->setXvel_Abs(10);
							player1->move(map);
							camera.x+=10;
							baseclass::coord.x+=10;
							if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
							
					}else if(player1->getRect()->y>=tmprect.y+tmprect.h-15&&player1->getRect()->y<=tmprect.y+tmprect.h){//Chequeamos colision por abajo
						player1->setHealth(player1->getHealth()-1);
						player1->setY(player1->getRect()->y+10);
						player1->setYvel(0);
						player1->setYvel_Abs(0);
					}
				}
				
			}
		}
		
		
		for(int i=0;i<movCubes.size();i++)movCubes[i]->refresh();//Actualizamos velocidades
		
		//Colisiones entre jugadores y cubos moviles 
		player1->setCFlag(false);
		for(int i=0;i<movCubes.size();i++){
			if(movCubes[i]->getRect()->x+movCubes[i]->getRect()->w-baseclass::coord.x>=0&&movCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH&&movCubes[i]->getRect()->y+movCubes[i]->getRect()->h-baseclass::coord.y>=0&&movCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT){//Solo chequeamos colisiones si el cubo esta en la pantalla
				SDL_Rect tmprect={movCubes[i]->getRect()->x-baseclass::coord.x,movCubes[i]->getRect()->y-baseclass::coord.y,TILE_SIZE,TILE_SIZE};//Creamos un rectangulo en coordenadas relativas a la pantalla
				
				if(collision(player1->getRect(),&tmprect)){
					if(player1->getRect()->y+player1->getRect()->h<tmprect.y+tmprect.h/2){//Colision arriba
						player1->setGroundTrue();
						player1->setCFlag(true);
						player1->setYvel(movCubes[i]->getYvel());
						player1->setYvel_Abs(movCubes[i]->getYvel());
						//player1->setXvel(player1->getXvel()+movCubes[i]->getXvel());//Esto no lo necesitamos, movemos la pantalla, no al jugador
						player1->setXvel_Abs(player1->getXvel_Abs()+movCubes[i]->getXvel());
						camera.x+=movCubes[i]->getXvel();//Movemos la pantalla para seguir al jugador
						baseclass::coord.x+=movCubes[i]->getXvel();
						if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
						if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
						
						player1->setY(tmprect.y-player1->getRect()->h+1);//Esto puede dar problemas con las colisiones con el mapa
					}else if(player1->getRect()->x+player1->getRect()->w<tmprect.x+15){//Colision izquierda
						if(movCubes[i]->getXvel()<0){
							player1->setXvel(player1->getXvel()+movCubes[i]->getXvel());//Adaptamos la velocidad sumando la del bloque
							player1->setXvel_Abs(player1->getXvel_Abs()+movCubes[i]->getXvel());
							camera.x+=movCubes[i]->getXvel();//Movemos la pantalla para seguir al jugador
							baseclass::coord.x+=movCubes[i]->getXvel();
							if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
							if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
						}
						player1->setX(tmprect.x-player1->getRect()->w+1);
					}else if(player1->getRect()->x>tmprect.x+tmprect.w-15) {//Colision derecha
						if(movCubes[i]->getXvel()>0){
							player1->setXvel(player1->getXvel()+movCubes[i]->getXvel());//Adaptamos la velocidad sumando la del bloque
							player1->setXvel_Abs(player1->getXvel_Abs()+movCubes[i]->getXvel());
							camera.x+=movCubes[i]->getXvel();//Movemos la pantalla para seguir al jugador
							baseclass::coord.x+=movCubes[i]->getXvel();
							if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
							if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
						}
						player1->setX(tmprect.x+tmprect.w-1);
					}else if(player1->getRect()->y>tmprect.y+tmprect.h-21){
						if(movCubes[i]->getYvel()>0){
							player1->setYvel(movCubes[i]->getYvel());
							player1->setYvel_Abs(movCubes[i]->getYvel());
						}
						player1->setY(tmprect.y+tmprect.h-1);//De nuevo esto puede dar problemas con las colisiones con el mapa, pero se puede eliminar con un correcto diseño de niveles
					}
				}
			}
		}
		//Colisiones entre jugadores y cubos que se caen
		for(int i=0;i<fallingCubes.size();i++){
			if(fallingCubes[i]->getRect()->x+fallingCubes[i]->getRect()->w-baseclass::coord.x>=0&&fallingCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH&&fallingCubes[i]->getRect()->y+fallingCubes[i]->getRect()->h-baseclass::coord.y>=0&&fallingCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT){//Solo chequeamos colisiones si el cubo esta en la pantalla
				SDL_Rect tmprect={fallingCubes[i]->getRect()->x-baseclass::coord.x,fallingCubes[i]->getRect()->y-baseclass::coord.y,TILE_SIZE,TILE_SIZE};//Creamos un rectangulo en coordenadas relativas a la pantalla
				
				if(collision(player1->getRect(),&tmprect)){
					if(player1->getRect()->y+player1->getRect()->h<tmprect.y+17){//Colision por arriba (la conflictiva)
						player1->setGroundTrue();
						player1->setCFlag(true);
						player1->setY(tmprect.y-player1->getRect()->h+1);
						if(fallingCubes[i]->getFalling()){
							player1->setYvel(fallingCubes[i]->getYvel());
							player1->setYvel_Abs(fallingCubes[i]->getYvel());
						}else{
							player1->setYvel(fallingCubes[i]->getYvel());
							player1->setYvel_Abs(fallingCubes[i]->getYvel());
							fallingCubes[i]->setFalling(true);
						}
					}else if(player1->getRect()->x+player1->getRect()->w<tmprect.x+15){//Colision izquierda
						player1->setX(tmprect.x-player1->getRect()->w);//Ponemos al jugador en el borde del cubo y reducimos su velocidad en x a 0
						player1->setXvel(0);
						player1->setXvel_Abs(0);
					}else if(player1->getRect()->x>tmprect.x+tmprect.w-15) {//Colision derecha
						player1->setX(tmprect.x+tmprect.w);//Ponemos al jugador en el borde del cubo y reducimos su velocidad en x a 0
						player1->setXvel(0);
						player1->setXvel_Abs(0);
					}else if(player1->getRect()->y>tmprect.y+tmprect.h-21){//Colision abajo
						if(!fallingCubes[i]->getFalling()){//Solo colisionamos por abajo si no esta cayendo para evitar glitches
							player1->setY(tmprect.y+tmprect.h);//Lo mismo de antes pero en el eje y
							player1->setYvel(0);
							player1->setYvel_Abs(0);
						}
					}
				}
			}
		}
		
		//Colisiones entre jugador y bloques de hielo
		bool mf=true;//Esto es para evitar que el movimiento se acelere cuando hay dos bloques juntos
		bool ice=false;//Esta variable nos dirá si ha habido una colisión con un bloque de hielo
		for(int i=0;i<iceCubes.size();i++){
			if(iceCubes[i]->getRect()->x+iceCubes[i]->getRect()->w-baseclass::coord.x>=0&&iceCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH&&iceCubes[i]->getRect()->y+iceCubes[i]->getRect()->h-baseclass::coord.y>=0&&iceCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT){//Solo chequeamos colisiones si el cubo esta en la pantalla
				SDL_Rect tmprect={iceCubes[i]->getRect()->x-baseclass::coord.x,iceCubes[i]->getRect()->y-baseclass::coord.y,TILE_SIZE,TILE_SIZE};//Creamos un rectangulo en coordenadas relativas a la pantalla
				if(player1->getRect()->y+player1->getRect()->h<tmprect.y+1&&player1->getRect()->y+player1->getRect()->h>tmprect.y-1&&player1->getRect()->x<=tmprect.x+tmprect.w&&player1->getRect()->x+player1->getRect()->w>=tmprect.x){
					ice=true;
					if(!keyPressed){
						if(player1->getDirection()=='l'){
							if(playerVel<0)playerVel+=0.2;
							else playerVel=0.0;
							//player1->setMoving(0);
							if(player1->getRect()->x>150){
								player1->setXvel((int)(playerVel+0.5));	
								player1->setXvel_Abs((int)(playerVel+0.5));			
							}else{
								player1->setXvel(0);
								player1->setXvel_Abs((int)(playerVel+0.5));
								if(mf){
									camera.x+=(int)(playerVel+0.5);
									baseclass::coord.x+=(int)(playerVel+0.5);
									mf=false;
								}
							}
								if(camera.x<0)camera.x=5000-SCREEN_WIDTH;
							
						}else if(player1->getDirection()=='r'){
							if(playerVel>0)playerVel-=0.2;
							else playerVel=0.0;
							//player1->setMoving(0);
							if(player1->getRect()->x<150){
								player1->setXvel((int)(playerVel-0.5));
								player1->setXvel_Abs((int)(playerVel-0.5));				
							}else{
								player1->setXvel(0);
								player1->setXvel_Abs((int)(playerVel-0.5));
								if(mf){
									camera.x+=(int)(playerVel-0.5);
									baseclass::coord.x+=(int)(playerVel-0.5);
									mf=false;
								}
							}
								if(camera.x>=5000-SCREEN_WIDTH)camera.x=0;
						}
					}else{
						if(player1->getDirection()=='l'&&playerVel>-5.0)playerVel-=0.1;
						else if(player1->getDirection()=='r'&&playerVel<5.0)playerVel+=0.1;
					}
				}
			}
		}
		
		player1->setOnIce(ice);
		
		//Aqui realizare la comprobación de colisiones con los bloques con moho
		bool flag=true;
		for(int i=0;i<mossCubes.size();i++){
			if(mossCubes[i]->getRect()->x+mossCubes[i]->getRect()->w-baseclass::coord.x>=0&&mossCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH&&mossCubes[i]->getRect()->y+mossCubes[i]->getRect()->h-baseclass::coord.y>=0&&mossCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT){//Solo chequeamos colisiones si el cubo esta en la pantalla
				SDL_Rect tmprect={mossCubes[i]->getRect()->x-baseclass::coord.x,mossCubes[i]->getRect()->y-baseclass::coord.y,TILE_SIZE,TILE_SIZE};//Creamos un rectangulo en coordenadas relativas a la pantalla
				if(mossCubes[i]->getRotation()){
					if(player1->getRect()->x+player1->getRect()->w<tmprect.x+3&&player1->getRect()->x+player1->getRect()->w>tmprect.x-3&&player1->getRect()->y+player1->getRect()->h>tmprect.y&&player1->getRect()->y<tmprect.y+tmprect.h){//Comprobamos la colision con el lado izq del bloque
						flag=false;
						player1->setMoving(0);
						player1->setYvel(3);
						player1->setYvel_Abs(3);
						player1->setSecondJump();
						player1->setDirection('n');
						player1->setMossy(true);
					}
			
				}else{
					if(player1->getRect()->x<tmprect.x+tmprect.w+3&&player1->getRect()->x>tmprect.x+tmprect.w-3&&player1->getRect()->y+player1->getRect()->h>tmprect.y&&player1->getRect()->y<tmprect.y+tmprect.h){//Comprobamos la colision con el lado der del bloque
						flag=false;
						player1->setMoving(0);
						player1->setYvel(3);
						player1->setYvel_Abs(3);
						player1->setSecondJump();
						player1->setDirection('m');
						player1->setMossy(true);
					}
				}
			}
		}
		
		if(flag)player1->setMossy(false);
		
		//Colisiones con cubos de goma
		for(int i=0;i<rubberCubes.size();i++){
			if(rubberCubes[i]->getRect()->x+rubberCubes[i]->getRect()->w-baseclass::coord.x>=0&&rubberCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH&&rubberCubes[i]->getRect()->y+rubberCubes[i]->getRect()->h-baseclass::coord.y>=0&&rubberCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT){//Solo chequeamos colisiones si el cubo esta en la pantalla
				SDL_Rect tmprect={rubberCubes[i]->getRect()->x-baseclass::coord.x,rubberCubes[i]->getRect()->y-baseclass::coord.y,TILE_SIZE,TILE_SIZE};//Creamos un rectangulo en coordenadas relativas a la pantalla
				if(player1->getRect()->y+player1->getRect()->h<tmprect.y+1&&player1->getRect()->y+player1->getRect()->h>tmprect.y-1&&player1->getRect()->x<=tmprect.x+tmprect.w&&player1->getRect()->x+player1->getRect()->w>=tmprect.x){//colision de los pies del jugador con la parte superior del cubo
					player1->setGroundTrue();
					player1->setJump(25);
					break;//Salimos del bucle, ya que solo queremos hacer el salto una vez incluso si clisionamos con varios a la vez
				}
			}
		}
		
		//Colisiones entre enemigos y obstaculos
		for(int i=0;i<enemies.size();i++){
			if(!enemies[i]->getStomp()&&!enemies[i]->getHit()){
				enemies[i]->enemy::obstacleCollision(obstacles);
			}
		}
		
		
		
		for(int i=0;i<bullets.size();i++){
			//Borramos las balas que salen de posjugador+-ancho_de_pantalla
			if(bullets[i]->getRect()->x >= player1->getRect()->x + SCREEN_WIDTH||bullets[i]->getRect()->x == 0||bullets[i]->getRect()->y==0||bullets[i]->getRect()->y+bullets[i]->getRect()->h>=player1->getRect()->y+SCREEN_HEIGHT){
				delete bullets[i];
				bullets.erase(bullets.begin()+i);	
			}
		}
		for(int i=0;i<bullets.size();i++){
			//Colisiones entre balas y obstaculos
			if(bullets[i]->bullet::obstacleCollision(obstacles)){
				delete bullets[i];//borramos de la memoria y del vector
				bullets.erase(bullets.begin()+i);
			}
		}
		for(int i=0;i<bullets.size();i++){
			//Colisiones entre balas y cubos moviles
			if(bullets[i]->bullet::movCubeCollision(movCubes)){
				delete bullets[i];//borramos de la memoria y del vector
				bullets.erase(bullets.begin()+i);
			}
		}
		for(int i=0;i<bullets.size();i++){
			//Colisiones entre balas y cubos que se caen
			if(bullets[i]->bullet::fallCubeCollision(fallingCubes)){
				delete bullets[i];//borramos de la memoria y del vector
				bullets.erase(bullets.begin()+i);
			}
		}
		
		//Chequeos de balas de enemigos
		for(int i=0;i<enemyBullets.size();i++){
			//Colisiones entre balas y obstaculos
			if(enemyBullets[i]->enemyBullet::obstacleCollision(obstacles)){
				delete enemyBullets[i];//borramos de la memoria y del vector
				enemyBullets.erase(enemyBullets.begin()+i);
			}
		}
		for(int i=0;i<enemyBullets.size();i++){
			//Colisiones entre balas y cubos moviles
			if(enemyBullets[i]->enemyBullet::movCubeCollision(movCubes)){
				delete enemyBullets[i];//borramos de la memoria y del vector
				enemyBullets.erase(enemyBullets.begin()+i);
			}
		}
		for(int i=0;i<enemyBullets.size();i++){
			//Colisiones entre balas y cubos que se caen
			if(enemyBullets[i]->enemyBullet::fallCubeCollision(fallingCubes)){
				delete enemyBullets[i];//borramos de la memoria y del vector
				enemyBullets.erase(enemyBullets.begin()+i);
			}
		}
		//Comprobamos colisiones etre disparos
		for(int i=0;i<enemyBullets.size();i++){
			for(int e=0;e<bullets.size();e++){
				if(enemyBullets[i]->getRect()->x+enemyBullets[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&enemyBullets[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&enemyBullets[i]->getRect()->y+enemyBullets[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&enemyBullets[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){
					SDL_Rect tmprect={enemyBullets[i]->getRect()->x-baseclass::coord.x,enemyBullets[i]->getRect()->y-baseclass::coord.y,20,20};
					if(collision(bullets[e]->getRect(),&tmprect)){
						delete enemyBullets[i];//borramos de la memoria y del vector
						enemyBullets.erase(enemyBullets.begin()+i);
						delete bullets[e];//borramos de la memoria y del vector
						bullets.erase(bullets.begin()+e);
						break;
					}
				}
			}
		}
		
		//Chequeos de balas de torreta
		for(int i=0;i<smallBullets.size();i++){
			//Colisiones entre balas y obstaculos
			if(smallBullets[i]->smallBullet::obstacleCollision(obstacles)){
				delete smallBullets[i];//borramos de la memoria y del vector
				smallBullets.erase(smallBullets.begin()+i);
			}
		}
		for(int i=0;i<smallBullets.size();i++){
			//Colisiones entre balas y cubos moviles
			if(smallBullets[i]->smallBullet::movCubeCollision(movCubes)){
				delete smallBullets[i];//borramos de la memoria y del vector
				smallBullets.erase(smallBullets.begin()+i);
			}
		}
		for(int i=0;i<smallBullets.size();i++){
			//Colisiones entre balas y cubos que se caen
			if(smallBullets[i]->smallBullet::fallCubeCollision(fallingCubes)){
				delete smallBullets[i];//borramos de la memoria y del vector
				smallBullets.erase(smallBullets.begin()+i);
			}
		}
		//Comprobamos colisiones etre disparos
		for(int i=0;i<smallBullets.size();i++){
			for(int e=0;e<bullets.size();e++){
				if(smallBullets[i]->getRect()->x+smallBullets[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&smallBullets[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&smallBullets[i]->getRect()->y+smallBullets[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&smallBullets[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){
					SDL_Rect tmprect={smallBullets[i]->getRect()->x-baseclass::coord.x,smallBullets[i]->getRect()->y-baseclass::coord.y,9,6};
					if(collision(bullets[e]->getRect(),&tmprect)){
						delete smallBullets[i];//borramos de la memoria y del vector
						smallBullets.erase(smallBullets.begin()+i);
						delete bullets[e];//borramos de la memoria y del vector
						bullets.erase(bullets.begin()+e);
						break;
					}
				}
			}
		}
		
		//Comprobamos colisiones entre disparos y minas
		for(int i=0;i<mines.size();i++){
			for(int e=0;e<bullets.size();e++){
				if(mines[i]->getRect()->x+mines[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&mines[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&mines[i]->getRect()->y+mines[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&mines[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){
					SDL_Rect tmprect={mines[i]->getRect()->x-baseclass::coord.x,mines[i]->getRect()->y-baseclass::coord.y,mines[i]->getRect()->w,mines[i]->getRect()->h};
					if(collision(bullets[e]->getRect(),&tmprect)){
						mines[i]->hit();//Activamos la bandera de hit
						delete bullets[e];//borramos de la memoria y del vector
						bullets.erase(bullets.begin()+e);
						break;
					}
				}
			}
		}
		
		
		for(int i=0;i<movCubes.size();i++)movCubes[i]->move();
		for(int i=0;i<fallingCubes.size();i++)fallingCubes[i]->move();
		player1->move(map);//Actualizamos la posición del jugador 
		if(player1->getOnIce()&&player1->getXcol())playerVel=0;
		
		for(int i=0;i<bullets.size();i++)bullets[i]->move(player1->getXvel_Abs(),player1->getYvel_Abs());//Actualizamos la posición de las balas
		
		for(int i=0;i<enemyBullets.size();i++)enemyBullets[i]->move(player1->getRect()->x,player1->getRect()->y);
		
		for(int i=0;i<smallBullets.size();i++)smallBullets[i]->move();
		
		//Mejor si quitamos el margen vertical y movemos solo las minas que esten en la misma altura de camara, asi evitamos problemas de colisiones
		for(int i=0;i<mines.size();i++){
			if(mines[i]->getRect()->x+mines[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&mines[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&mines[i]->getRect()->y+mines[i]->getRect()->h-baseclass::coord.y>0&&mines[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT){
				if(mines[i]->move(player1->getRect()->x,player1->getRect()->y,map,obstacles,movCubes,fallingCubes)){
					delete mines[i];//borramos de la memoria y del vector
					mines.erase(mines.begin()+i);
				}
			}
		}
		
		for(int i=0;i<enemies.size();i++){
			if(!enemies[i]->getStomp()&&!enemies[i]->getHit()&&enemies[i]->getRect()->x>baseclass::coord.x-2*TILE_SIZE && enemies[i]->getRect()->x<baseclass::coord.x+baseclass::coord.w+2*TILE_SIZE && enemies[i]->getRect()->y>baseclass::coord.y && enemies[i]->getRect()->y<baseclass::coord.y+SCREEN_HEIGHT){
				enemies[i]->move(map);//Actualizamos la posición de los enemigos que estan dentro de la pantalla, los que estan fuera se quedan fijos
			}else if(enemies[i]->getStomp()||enemies[i]->getHit())enemies[i]->increaseTimeCounter();
		 }
		 
		
		
		//Aqui mostramos las cosas por pantalla
		
		SDL_BlitSurface(background,&camera,screen,NULL);
		
		player1->show(screen);
		
		for(int i=0;i<bullets.size();i++)bullets[i]->show(screen);//Mostramos las balas en pantalla
		for(int i=0;i<enemyBullets.size();i++)enemyBullets[i]->show(screen);//Mostramos las balas en pantalla
		for(int i=0;i<smallBullets.size();i++)smallBullets[i]->show(screen);//etc
		for(int i=0;i<enemies.size();i++){
			if(enemies[i]->getRect()->x+enemies[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&enemies[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&enemies[i]->getRect()->y+enemies[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&enemies[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo mostramos enemigos dentro de la pantalla 
				enemies[i]->show(screen);//Mostramos los enemigos por pantalla
			}
		}
		for(int i=0;i<cannons.size();i++){
			if(cannons[i]->getRect()->x+cannons[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&cannons[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&cannons[i]->getRect()->y+cannons[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&cannons[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo mostramos enemigos dentro de la pantalla 
				cannons[i]->show(screen);//Mostramos los cañones por pantalla
			}
		}
		for(int i=0;i<enemyBullets.size();i++){
			if(enemyBullets[i]->getRect()->x+enemyBullets[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&enemyBullets[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&enemyBullets[i]->getRect()->y+enemyBullets[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&enemyBullets[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo mostramos enemigos dentro de la pantalla 
				enemyBullets[i]->show(screen);//Mostramos los disparos de los enemigos por pantalla
			}
		}
		for(int i=0;i<turrets.size();i++){
			if(turrets[i]->getRect()->x+turrets[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&turrets[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&turrets[i]->getRect()->y+turrets[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&turrets[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo mostramos enemigos dentro de la pantalla 
				turrets[i]->show(screen);//Mostramos los cañones por pantalla
			}
		}
		for(int i=0;i<mines.size();i++){
			if(mines[i]->getRect()->x+mines[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&mines[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&mines[i]->getRect()->y+mines[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&mines[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo mostramos enemigos dentro de la pantalla 
				mines[i]->show(screen);//Mostramos las minas por pantalla
			}
		}
		for(int i=0;i<obstacles.size();i++){
			if(obstacles[i]->getRect()->x+obstacles[i]->getRect()->w-baseclass::coord.x+2*TILE_SIZE>0&&obstacles[i]->getRect()->x-baseclass::coord.x<SCREEN_WIDTH+2*TILE_SIZE&&obstacles[i]->getRect()->y+obstacles[i]->getRect()->h-baseclass::coord.y+2*TILE_SIZE>0&&obstacles[i]->getRect()->y-baseclass::coord.y<SCREEN_HEIGHT+2*TILE_SIZE){//Solo mostramos obstaculos dentro de la pantalla 	
				obstacles[i]->show(screen);//Mostramos los obstaculos por pantalla
			}
		}
		for(int i=0;i<movCubes.size();i++){
			if(movCubes[i]->getRect()->x+movCubes[i]->getRect()->w-baseclass::coord.x>=-2*TILE_SIZE&&movCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH+2*TILE_SIZE&&movCubes[i]->getRect()->y+movCubes[i]->getRect()->h-baseclass::coord.y>=-2*TILE_SIZE&&movCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos colisiones si el cubo esta en la pantalla
				movCubes[i]->show(screen);//Mostramos los cubos moviles por pantalla
			}
		}
		
		showmap();
		
		for(int i=0;i<fallingCubes.size();i++){
			if(fallingCubes[i]->getRect()->x+fallingCubes[i]->getRect()->w-baseclass::coord.x>=-2*TILE_SIZE&&fallingCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH+2*TILE_SIZE&&fallingCubes[i]->getRect()->y+fallingCubes[i]->getRect()->h-baseclass::coord.y>=-2*TILE_SIZE&&fallingCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos colisiones si el cubo esta en la pantalla
				fallingCubes[i]->show(screen);//Mostramos los cubos que se caen por pantalla
			}
		}
		for(int i=0;i<iceCubes.size();i++){
			iceCubes[i]->actualizeFrame();
			if(iceCubes[i]->getRect()->x+iceCubes[i]->getRect()->w-baseclass::coord.x>=-2*TILE_SIZE&&iceCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH+2*TILE_SIZE&&iceCubes[i]->getRect()->y+iceCubes[i]->getRect()->h-baseclass::coord.y>=-2*TILE_SIZE&&iceCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos colisiones si el cubo esta en la pantalla
				iceCubes[i]->show(screen);//Mostramos los cubos de hielo por pantalla
			}
		}
		for(int i=0;i<mossCubes.size();i++){
			if(mossCubes[i]->getRect()->x+mossCubes[i]->getRect()->w-baseclass::coord.x>=-2*TILE_SIZE&&mossCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH+2*TILE_SIZE&&mossCubes[i]->getRect()->y+mossCubes[i]->getRect()->h-baseclass::coord.y>=-2*TILE_SIZE&&mossCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos colisiones si el cubo esta en la pantalla
				mossCubes[i]->show(screen);//Mostramos los cubos mohosos por pantalla
			}
		}
		
		for(int i=0;i<rubberCubes.size();i++){
			if(rubberCubes[i]->getRect()->x+rubberCubes[i]->getRect()->w-baseclass::coord.x>=-2*TILE_SIZE&&rubberCubes[i]->getRect()->x-baseclass::coord.x<=SCREEN_WIDTH+2*TILE_SIZE&&rubberCubes[i]->getRect()->y+rubberCubes[i]->getRect()->h-baseclass::coord.y>=-2*TILE_SIZE&&rubberCubes[i]->getRect()->y-baseclass::coord.y<=SCREEN_HEIGHT+2*TILE_SIZE){//Solo chequeamos colisiones si el cubo esta en la pantalla
				rubberCubes[i]->show(screen);//Mostramos los cubos mohosos por pantalla
			}
		}
		
		if(player1->getHealth()<=0||player1->getRect()->y>=SCREEN_HEIGHT+2*TILE_SIZE){
			player1->setHealth(0);
			return true;
		}//Con este codigo cerramos si la salud del jugador baja de 0 pero no hacemos nada mas
		
		//Aqui mostramos el contador de vida
		SDL_Rect counter={30,10,310,20};
		SDL_FillRect(screen,&counter,SDL_MapRGB(screen->format,0xaa,0xaa,0xaa));
		counter.x=35;
		counter.y=15;
		counter.h=10;
		counter.w=player1->getHealth()*30;
		SDL_FillRect(screen,&counter,SDL_MapRGB(screen->format,0xff,0x00,0x00));
		counter.x=5;
		counter.y=10;
		counter.h=10;
		counter.w=10;
		SDL_BlitSurface(simbol,NULL,screen,&counter);
		
		SDL_Flip(screen);//Mandamos las cosas a la pantalla
		
		
		SDL_Rect tmprect={finish.x-coord.x,finish.y-coord.y,50,50};
		if(collision(player1->getRect(),&tmprect)){
			lvClear=true;
			return true;
		}
		
		if ((1000/FPS)>(SDL_GetTicks()-start)){//si el bucle se ejecuta demasiado deprisa reducimos la velocidad de refresco
			SDL_Delay((1000/FPS)-(SDL_GetTicks()-start));//de esta manera reducimos el uso del procesador 
		}
	}
	return false;
}

void game::setMen(bool b){
	first=b;
}


