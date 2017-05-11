#include "otherPlayer.h"

otherPlayer::otherPlayer(std::vector<unsigned int> f,int i): frames(f),id(i),curframe(0) {cs.setR(3.0);}
void otherPlayer::show(){
	glPushMatrix();
	 glTranslatef(position.getX(),position.getY(),position.getZ());
	 glRotatef(rotation.getX(),1.0,0.0,0.0);
	 glRotatef(rotation.getY() + 90.0,0.0,1.0,0.0);
	 glRotatef(rotation.getZ(),0.0,0.0,1.0);
	 glScalef(1.0,1.0,1.0);
	 glCallList(frames[curframe]);
	glPopMatrix();
}
