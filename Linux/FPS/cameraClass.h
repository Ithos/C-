#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "vector3d.h"


class camera{
	vector3d loc;
	float camPitch,camYaw;
	float movevel;
	float mousevel;
	bool mi,isMoved,freeCam,moving;
	void lockCamera();
	void moveCamera(float dir,vector3d& newPos);
	void moveCameraUp(float dir,vector3d& newPos);
	public:
	camera(bool f=false);
	camera(vector3d l,bool f=false);
	camera(vector3d l,float yaw,float pitch,bool f=false);
	camera(vector3d l,float yaw,float pitch,float mv,float mov,bool f=false);
	vector3d control();
	void updateCamera();
	vector3d getVector();
	vector3d getLocation();
	float getPitch();
	float getYaw();
	float getMovevel();
	float getMousevel();
	bool isMouseIn();
	
	void setLocation(vector3d l);
	void lookAt(float pitch,float yaw);
	void mouseIn(bool b);
	void setSpeed(float mv,float mov);
	
	bool getMoving();
};


#endif
