#include "item.h"

specItem::specItem(const vector3d& rot,const vector3d& sc,const collisionSphere& css,int idn,unsigned int m){
	rotation=rot;
	scale=sc;
	cs=css;
	id=idn;
	mesh=m;
}

int specItem::getId(){
	return id;
}

vector3d specItem::getRot(){
	return rotation;
}

vector3d specItem::getScale(){
	return scale;
}

void specItem::setRot(const vector3d& rot){
	rotation=rot;
}

collisionSphere specItem::getPos(){
	return cs;
}

unsigned int specItem::getMesh(){
	return mesh;
}

void item::add(const vector3d& rot,const vector3d& sc,const collisionSphere& css,int idn,unsigned int m){
	items.push_back(specItem(rot,sc,css,idn,m));
}

void item::del(int id){
	for(int i=0;i<items.size();i++){
		if(items[i].getId()==id){
			items.erase(items.begin()+i);
			break;
		}
	}
}

int item::update(collisionSphere player){
	for(int i=0;i<items.size();i++){
		items[i].setRot(items[i].getRot()+vector3d(0,1,0));
		if(items[i].getRot().getY()>=360)items[i].setRot(items[i].getRot()-vector3d(0,360,0));
		if(collision::spheresphere(player.getCenter(),player.getR(),items[i].getPos().getCenter(),items[i].getPos().getR())){
			int tmp=items[i].getId();
			items.erase(items.begin()+i);
			return tmp;
		}
	}
	return -1;
}

void item::show(){
	for(int i=0;i<items.size();i++){
		glPushMatrix();
			glTranslatef(items[i].getPos().getCenter().getX(),items[i].getPos().getCenter().getY(),items[i].getPos().getCenter().getZ());
			glRotatef(items[i].getRot().getX(),1,0,0);
			glRotatef(items[i].getRot().getY(),0,1,0);
			glRotatef(items[i].getRot().getZ(),0,0,1);
			glScalef(items[i].getScale().getX(),items[i].getScale().getY(),items[i].getScale().getZ());
			glCallList(items[i].getMesh());
		glPopMatrix();
	}
}
