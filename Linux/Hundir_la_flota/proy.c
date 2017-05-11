#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct casilla{
		char num[2];
		int barco;
		int tocado;
		};
		

void printfield(struct casilla campo1[10][10], struct casilla campo2[10][10]){
	int i,j;
	printf("\n\n\n");
	printf("  CPU Field               Player Field\n\n");
	printf("  A B C D E F G H I J     A B C D E F G H I J\n");
	for(i=0;i<10;i++){
		for(j=0;j<20;j++){
			if(j<10){
				if(j==0){
					if(!(campo1[i][j].barco||campo1[i][j].tocado))printf("%i *",i);
					if(!campo1[i][j].barco&&campo1[i][j].tocado)printf("%i @",i);
					if(campo1[i][j].barco&&campo1[i][j].tocado)printf("%i #",i);
					if(campo1[i][j].barco&&!campo1[i][j].tocado)printf("%i *",i);
				}
				if((j!=0)&&(j!=9)){
					if(!(campo1[i][j].barco||campo1[i][j].tocado))printf(" *");
					if(!campo1[i][j].barco&&campo1[i][j].tocado)printf(" @");
					if(campo1[i][j].barco&&campo1[i][j].tocado)printf(" #");
					if(campo1[i][j].barco&&!campo1[i][j].tocado)printf(" *");
				}
				if(j==9){
					if(!(campo1[i][j].barco||campo1[i][j].tocado))printf(" *   ");
					if(!campo1[i][j].barco&&campo1[i][j].tocado)printf(" @   ");
					if(campo1[i][j].barco&&campo1[i][j].tocado)printf(" #   ");
					if(campo1[i][j].barco&&!campo1[i][j].tocado)printf(" *   ");
				}
			}else{
				if(j==10){
					if(!(campo2[i][j-10].barco||campo2[i][j-10].tocado))printf("%i *",i);
					if(!campo2[i][j-10].barco&&campo2[i][j-10].tocado)printf("%i @",i);
					if(campo2[i][j-10].barco&&campo2[i][j-10].tocado)printf("%i #",i);
					if(campo2[i][j-10].barco&&!campo2[i][j-10].tocado)printf("%i %i",i,campo2[i][j-10].barco); 
				}
				if((j!=10)&&(j!=19)){
					if(!(campo2[i][j-10].barco||campo2[i][j-10].tocado))printf(" *");
					if(!campo2[i][j-10].barco&&campo2[i][j-10].tocado)printf(" @");
					if(campo2[i][j-10].barco&&campo2[i][j-10].tocado)printf(" #");
					if(campo2[i][j-10].barco&&!campo2[i][j-10].tocado)printf(" %i",campo2[i][j-10].barco);
				}
				if(j==19){
					if(!(campo2[i][j-10].barco||campo2[i][j-10].tocado))printf(" *\n");
					if(!campo2[i][j-10].barco&&campo2[i][j-10].tocado)printf(" @\n");
					if(campo2[i][j-10].barco&&campo2[i][j-10].tocado)printf(" #\n");
					if(campo2[i][j-10].barco&&!campo2[i][j-10].tocado)printf(" %i\n",campo2[i][j-10].barco);	
				}
			}
		}
	}
	
	}

int main()
{
	struct casilla campocpu[10][10];
	struct casilla campoplayer[10][10];
	int i,j,k,a,b,d,x,y,e=0,f=0;
	char pos[2],c;
	
	//Numeramos las casillas
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			campocpu[i][j].num[0]=48+i;
			campocpu[i][j].num[1]=65+j;
		}
	}
	
	
	//Generamos las posiciones de los barcos de forma aleatoria
	srand(time(NULL));
	a=0;
	while(a!=44){
		//inicializamos los campos barco y tocado a 0
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			campocpu[i][j].barco=0;
			campocpu[i][j].tocado=0;
		}
	}
	for (k=1;k<6;k++){
		i=rand()%10;
		j=rand()%10;
		if (campocpu[i][j].barco==0){
			if ((rand()%7)>3){
				if(k<4){
					for(a=k;(a-k)<(6-k);a++){
						if((10-i)>(6-k))campocpu[i+(a-k)][j].barco=k;
						if((10-i)<(6-k))campocpu[i-(a-k)][j].barco=k;
					}
				}
				if (k>3){
					for(a=k;(a-k)<(7-k);a++){
						if((10-i)>(6-k))campocpu[i+(a-k)][j].barco=k;
						if((10-i)<(6-k))campocpu[i-(a-k)][j].barco=k;
					}
				}
			}else{
				if(k<4){
					for(a=k;(a-k)<(6-k);a++){
						if((10-j)>(6-k))campocpu[i][j+(a-k)].barco=k;
						if((10-j)<(6-k))campocpu[i][j-(a-k)].barco=k;
					}
				}
				if (k>3){
					for(a=k;(a-k)<(7-k);a++){
						if((10-j)>(6-k))campocpu[i][j+(a-k)].barco=k;
						if((10-j)<(6-k))campocpu[i][j-(a-k)].barco=k;
					}
				}
			}
		}else k--;
	}
	//comprueba que los barcos estan bien colocados
	a=0;
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			a=a+campocpu[i][j].barco;
		}
	}
/*printf("%i\n",a);*/
}

	/*for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(j!=9)printf("%i ",campocpu[i][j].barco);
			if(j==9)printf("%i\n",campocpu[i][j].barco);
		}
	}*/
	//Numeramos las casillas del campo del jugador
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			campoplayer[i][j].num[0]=48+i;
			campoplayer[i][j].num[1]=65+j;
		}
	}
	/*for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			printf("%c",campocpu[i][j].num[0]);
			printf("%c ",campocpu[i][j].num[1]);
		}
	}
	printf("\n\n");
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			printf("%c",campoplayer[i][j].num[0]);
			printf("%c ",campoplayer[i][j].num[1]);
		}
	}*/
	
	
	//inicializamos los campos barco y tocado a 0 del campo del jugador
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			campoplayer[i][j].barco=0;
			campoplayer[i][j].tocado=0;
		}
	}
	
	printfield(campocpu,campoplayer);
	
	
	//Pedimos al jugador la posiscion de sus barcos
	k=0;b=0;
	for(d=0;d<5;d++){
	while(k==0){
		b=0;
			while(b==0){
				b=0;
				if(d==0)printf("\n\nIntroduce la posiscion inicial del portaaviones (5 casillas Fila/Columna):");
				if(d==1)printf("\n\nIntroduce la posiscion inicial del destructor (4 casillas Fila/Columna):");
				if(d==2)printf("\n\nIntroduce la posiscion inicial del crucero (3 casillas Fila/Columna):");
				if(d==3)printf("\n\nIntroduce la posiscion inicial del submarino (3 casillas Fila/Columna):");
				if(d==4)printf("\n\nIntroduce la posiscion inicial de la lancha (2 casillas Fila/Columna):");
				gets(pos);
				for(i=0;i<10;i++){
					for(j=0;j<10;j++){
						if((campoplayer[i][j].num[0]==pos[0])&&(campoplayer[i][j].num[1]==pos[1])&&(campoplayer[i][j].barco!=0))printf("\nPosicion incorrecta");
						if((campoplayer[i][j].num[0]==pos[0])&&(campoplayer[i][j].num[1]==pos[1])&&(campoplayer[i][j].barco==0))b=1;
					}
				}
			}
			b=0;
			printf("\nPulsa V para colocarlo en posicion vertical o cualquier otra para colocarlo en posicion horizontal:");
			c=getchar();
			if(c=='V'){
				for(i=0;i<10;i++){
					for(j=0;j<10;j++){
						if(d<3){
						if((campoplayer[i][j].num[0]==pos[0])&&(campoplayer[i][j].num[1]==pos[1])){
							if((i+(5-d))>10){printf("\nPosicion incorrecta");b=1;}
						    if(b==0){
								for(a=0;a<(5-d);a++){
									if(campoplayer[i+a][j].barco!=0){
										if(b==0){printf("\nPosicion incorrecta");b=1;}
									}
								}
							}
							if(b==0){
								for(a=0;a<(5-d);a++){
									if(d==0)campoplayer[i+a][j].barco=1;
									if(d==1)campoplayer[i+a][j].barco=2;
									if(d==2)campoplayer[i+a][j].barco=3;
									if(d==3)campoplayer[i+a][j].barco=4;
									if(d==4)campoplayer[i+a][j].barco=5;
									k=1;
								}
							}
						}
					}
					if(d>2){
						if((campoplayer[i][j].num[0]==pos[0])&&(campoplayer[i][j].num[1]==pos[1])){
							if((i+(6-d))>10){printf("\nPosicion incorrecta");b=1;}
						    if(b==0){
								for(a=0;a<(4-d);a++){
									if(campoplayer[i+a][j].barco!=0){
										if(b==0){printf("\nPosicion incorrecta");b=1;}
									}
								}
							}
							if(b==0){
								for(a=0;a<(6-d);a++){
									if(d==0)campoplayer[i+a][j].barco=1;
									if(d==1)campoplayer[i+a][j].barco=2;
									if(d==2)campoplayer[i+a][j].barco=3;
									if(d==3)campoplayer[i+a][j].barco=4;
									if(d==4)campoplayer[i+a][j].barco=5;
									k=1;
								}
							}
						}
					}
					}
				}
			}
			if(c!='V'){
				for(i=0;i<10;i++){
					for(j=0;j<10;j++){
						if(d<3){
						if((campoplayer[i][j].num[0]==pos[0])&&(campoplayer[i][j].num[1]==pos[1])){
							if((j+(5-d))>10){printf("\nPosicion incorrecta");b=1;}
						    if(b==0){
								for(a=0;a<(5-d);a++){
									if(campoplayer[i][j+a].barco!=0){
										if(b==0){printf("\nPosicion incorrecta");b=1;}
									}
								}
							}
							if(b==0){
								for(a=0;a<(5-d);a++){
									if(d==0)campoplayer[i][j+a].barco=1;
									if(d==1)campoplayer[i][j+a].barco=2;
									if(d==2)campoplayer[i][j+a].barco=3;
									if(d==3)campoplayer[i][j+a].barco=4;
									if(d==4)campoplayer[i][j+a].barco=5;
									k=1;
								}
							}
						}
					}
					if(d>2){
						if((campoplayer[i][j].num[0]==pos[0])&&(campoplayer[i][j].num[1]==pos[1])){
							if((j+(6-d))>10){printf("\nPosicion incorrecta");b=1;}
						    if(b==0){
								for(a=0;a<(4-d);a++){
									if(campoplayer[i][j+a].barco!=0){
										if(b==0){printf("\nPosicion incorrecta");b=1;}
									}
								}
							}
							if(b==0){
								for(a=0;a<(6-d);a++){
									if(d==0)campoplayer[i][j+a].barco=1;
									if(d==1)campoplayer[i][j+a].barco=2;
									if(d==2)campoplayer[i][j+a].barco=3;
									if(d==3)campoplayer[i][j+a].barco=4;
									if(d==4)campoplayer[i][j+a].barco=5;
									k=1;
								}
							}
						}
					}
					}
				}
			}
		}k=0;
	
			
		system("clear");
		printfield(campocpu,campoplayer);
	}
	//Pedimos al jugador que introduzca las coordenadas a bombardear
	k=0;b=0;c=1;
	while(k==0){
		b=0;c=1;
		while(b==0){
			printf("\nIntroduzca las coordenadas a atacar(fila/columna):\n");
			gets(pos);
			for(i=0;i<10;i++){
				for(j=0;j<10;j++){
					if((campocpu[i][j].num[0]==pos[0])&&(campocpu[i][j].num[1]==pos[1])){
						if(campocpu[i][j].tocado==1)printf("Coordenadas repetidas, elija otras.\n");
						if(campocpu[i][j].tocado==0){
							b=1;
							campocpu[i][j].tocado=1;
							system("clear");
							printfield(campocpu,campoplayer);
							if(campocpu[i][j].barco==0)printf("Agua.\n");
							if(campocpu[i][j].barco!=0){
								for(a=0;a<10;a++){
									for(d=0;d<10;d++){
										/*printf("%i",campocpu[a][d].barco);
										printf("%i",campocpu[i][j].barco);
										printf("%i",campocpu[a][d].tocado);*/
										
										if((campocpu[a][d].barco==campocpu[i][j].barco)&&(campocpu[a][d].tocado!=1))c=2;
									}
								}
								if(c!=2)printf("Hundido.\n");
								if(c==2)printf("Tocado.\n");
							}								
						}
					}
				}
			}
		}
		k=1;
		for(i=0;i<10;i++){
				for(j=0;j<10;j++){
					if((campocpu[i][j].barco!=0)&&(campocpu[i][j].tocado==0))k=0;
				}
			}
		if(k==1)printf("Enhorabuena, ha destruido todos los barcos.\n");
		//La CPU dispara
		printf("Pulsa intro para continuar");
		getchar();
		b=0;c=1;
		if(k==0){
			if(e==0){
				c=1;
				while(b==0){
					for(i=0;i<10;i++){
						for(j=0;j<10;j++){
							if((campoplayer[i][j].tocado!=0)&&(campoplayer[i][j].barco!=0)&&(b==0)){
								for(x=0;x<10;x++){
									for(y=0;y<10;y++){
										if((campoplayer[x][y].barco==campoplayer[i][j].barco)&&(campoplayer[x][y].tocado==0)&&(b==0)){
											if((i>0)&&(b==0)&&(campoplayer[i-1][j].tocado!=0)&&(campoplayer[i-1][j].barco!=0)&&(campoplayer[i+1][j].tocado==0)){
												campoplayer[i+1][j].tocado=1;
												b=1;c=2;
												if(campoplayer[i+1][j].barco!=0)e=2;
											}
											if((i<9)&&(b==0)&&(campoplayer[i+1][j].tocado!=0)&&(campoplayer[i+1][j].barco!=0)&&(campoplayer[i-1][j].tocado==0)){
												campoplayer[i-1][j].tocado=1;
												b=1;c=2;
												if(campoplayer[i-1][j].barco!=0)e=3;
											}
											if((j>0)&&(b==0)&&(campoplayer[i][j-1].tocado!=0)&&(campoplayer[i][j-1].barco!=0)&&(campoplayer[i][j+1].tocado==0)){
												campoplayer[i][j+1].tocado=1;
												b=1;c=2;
												if(campoplayer[i][j+1].barco!=0)e=4;
											}
											if((j<9)&&(b==0)&&(campoplayer[i][j+1].tocado!=0)&&(campoplayer[i][j+1].barco!=0)&&(campoplayer[i][j-1].tocado==0)){
												campoplayer[i][j-1].tocado=1;
												b=1;c=2;
												if(campoplayer[i][j-1].barco!=0)e=5;
											}
										}
									}
								}
								if(c==2){x=i;y=j;}
								c=1;
							}
						}
					}
					if(b==0){
					x=rand()%10;
					y=rand()%10;
					if(campoplayer[x][y].tocado==0){
						campoplayer[x][y].tocado=1;b=1;
						if(campoplayer[x][y].barco!=0)e=1;
					}
				}
			}
		}
			if((e==1)&&(b==0)){
				if((campoplayer[x][y].tocado!=0)&&(campoplayer[x+1][y].tocado!=0)&&(campoplayer[x-1][y].tocado!=0)&&(campoplayer[x][y+1].tocado!=0)&&(campoplayer[x][y-1].tocado!=0)){
					while(b==0){
					x=rand()%10;
					y=rand()%10;
					if(campoplayer[x][y].tocado==0){
						campoplayer[x][y].tocado=1;b=1;
						if(campoplayer[x][y].barco!=0)e=1;
					}
				}
			}
				while(b==0){
					d=rand()%8;
					if((d<3)&&(campoplayer[x+1][y].tocado==0)&&(x<9)){
						campoplayer[x+1][y].tocado=1;b=1;
						if(campoplayer[x+1][y].barco!=0)e=2;
					}
					if((d>2)&&(d<5)&&(campoplayer[x-1][y].tocado==0)&&(x>0)){
						campoplayer[x-1][y].tocado=1;b=1;
						if(campoplayer[x-1][y].barco!=0)e=3;
					}
					if((d>4)&&(d<7)&&(campoplayer[x][y+1].tocado==0)&&(y<9)){
						campoplayer[x][y+1].tocado=1;b=1;
						if(campoplayer[x][y+1].barco!=0)e=4;
					}
					if((d>6)&&(campoplayer[x][y-1].tocado==0)&&(y>0)){
						campoplayer[x][y-1].tocado=1;b=1;
						if(campoplayer[x][y-1].barco!=0)e=5;
					}d=0;
				}
			}
			c=1;
			if((e==2)&&(b==0)){
				if((campoplayer[x+2+f][y].tocado!=0)||((x+2+f)>9)){e=3;f=0;}										
				if((campoplayer[x+2+f][y].tocado==0)&&(e==2)){
					campoplayer[x+2+f][y].tocado=1;b=1;
					if(campoplayer[x+2+f][y].barco!=0){
						for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x+2+f][y].barco)&&(campoplayer[i][j].tocado==0))c=2;
							}
						}
						if(c!=1)f++;
						if(c==1){e=0;f=0;}					
					}
					if(campoplayer[x+2+f][y].barco==0){
						for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x+1+f][y].barco)&&(campoplayer[i][j].tocado==0)){e=3;f=0;}
							}
						}
						if(e==2){e=0;f=0;}
					
					}
			}
			
			}
			c=1;
			if((e==3)&&(b==0)){	
				if((campoplayer[x-2-f][y].tocado!=0)||((x-2-f)<0)){
					i=0;
					while((campoplayer[x+i][y].tocado!=0)&&((x+i)<10))i++;
					if(campoplayer[x+i][y].tocado==0){
					campoplayer[x+i][y].tocado=1;
					b=1;
					if(campoplayer[x+i][y].barco==0){e=0;f=0;}
					if(campoplayer[x+i][y].barco!=0){x=x+i;e=1;f=0;}
				}
					if(campoplayer[x+i][y].tocado!=0){
						while(b==0){
						x=rand()%10;
						y=rand()%10;
						if(campoplayer[x][y].tocado==0){
							campoplayer[x][y].tocado=1;b=1;
							if(campoplayer[x][y].barco!=0)e=1;
						}
					}					
				}
				
			}
			if((b==0) && (campoplayer[x-2-f][y].tocado==0)){
				campoplayer[x-2-f][y].tocado=1;b=1;
				if(campoplayer[x-2-f][y].barco!=0){
					for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x-2-f][y].barco)&&(campoplayer[i][j].tocado==0))c=2;
							}
						}
						if(c!=1)f++;
						if(c==1){e=0;f=0;}					
				}
				if(campoplayer[x-2-d][y].barco==0){
						for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x-1-d][y].barco)&&(campoplayer[i][j].tocado==0)){e=2;f=0;}
							}
						}
						if(e==3){e=0;f=0;}
					
					}
				}			
			}
			c=1;
			if((e==4)&&(b==0)){
				if((campoplayer[x][y+2+f].tocado!=0)||((y+2+f)>9)){e=5;f=0;}										
				if((campoplayer[x][y+2+f].tocado==0)&&(e==4)){
					campoplayer[x][y+2+f].tocado=1;b=1;
					if(campoplayer[x][y+2+f].barco!=0){
						for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x][y+2+f].barco)&&(campoplayer[i][j].tocado==0))c=2;
							}
						}
						if(c!=1)f++;
						if(c==1){e=0;f=0;}					
					}
					if(campoplayer[x][y+2+f].barco==0){
						for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x][y+1+f].barco)&&(campoplayer[i][j].tocado==0)){e=5;f=0;}
							}
						}
						if(e==4){e=0;f=0;}
					
					}
				}				
			}
			c=1;
			if((e==5)&&(b==0)){	
				if((campoplayer[x][y-2-f].tocado!=0)||((y-2-f)<0)){
					i=0;
					while((campoplayer[x][y+i].tocado!=0)&&((y+i)<10))i++;
						if(campoplayer[x][y+i].tocado==0){
						campoplayer[x][y+i].tocado=1;
						b=1;
						if(campoplayer[x][y+i].barco==0){e=0;f=0;}
						if(campoplayer[x][y+i].barco!=0){y=y+i;e=1;f=0;}
					}
						if(campoplayer[x][y+i].tocado!=0){
							while(b==0){
							x=rand()%10;
							y=rand()%10;
							if(campoplayer[x][y].tocado==0){
								campoplayer[x][y].tocado=1;b=1;
								if(campoplayer[x][y].barco!=0)e=1;
							}
						}					
					}
			}
			if((b==0) && (campoplayer[x][y-2-f].tocado==0)){
				campoplayer[x][y-2-f].tocado=1;b=1;
				if(campoplayer[x][y-2-f].barco!=0){
					for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x][y-2-f].barco)&&(campoplayer[i][j].tocado==0))c=2;
							}
						}
						if(c!=1)f++;
						if(c==1){e=0;f=0;}					
				}
				if(campoplayer[x][y-2-f].barco==0){
						for(i=0;i<10;i++){
							for(j=0;j<10;j++){
								if((campoplayer[i][j].barco==campoplayer[x][y-1-f].barco)&&(campoplayer[i][j].tocado==0)){e=4;f=0;}
							}
						}
						if(e==5){e=0;f=0;}
					
					}
				}			
			}
		printfield(campocpu,campoplayer);
		k=1;
		for(i=0;i<10;i++){
				for(j=0;j<10;j++){
					if((campoplayer[i][j].barco!=0)&&(campoplayer[i][j].tocado==0))k=0;
				}
			}
		if(k==1){
			printf("Lastima, todos sus barcos han sido hundidos.\n");
			printf("Pulsa intro para continuar");
			getchar();
		}
				
		}
	}
			
				
			
			
	
	
	
		return 0;
}
