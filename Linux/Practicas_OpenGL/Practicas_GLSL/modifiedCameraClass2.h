#ifndef CAMERA_H
#define CAMERA_H
#include <cmath>
#include <iostream>
#include "matrices.h"
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "vector3d.h"



class camera{
	vector3d loc;
	float camPitch,camYaw;
	float movevel;
	float mousevel;
	bool mi,isMoved,free;
	void lockCamera();
	void moveCamera(float dir);
	void moveCameraUp(float dir);
	public:
	camera(bool f=false);
	camera(vector3d l,bool f=false);
	camera(vector3d l,float yaw,float pitch, bool f=false);
	camera(vector3d l,float yaw,float pitch,float mv,float mov, bool f=false);
	void control(matrices &pipeline);
	void updateCamera(matrices &pipeline);
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
};


#endif
