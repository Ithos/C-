#include "game.h"


int main (int argc, char** argv){
	
	game g;
	while(g.start())g.setMen(false);
	return 0;
}
