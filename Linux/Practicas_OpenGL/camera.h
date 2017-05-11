#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cmath>
#include "objLoader.h"

#ifndef CAMERA_H
#define CAMERA_H

void lockCamera();
void moveCamera(float,float);
void moveCameraUp(float,float);
void control(float,float,bool);
void updateCamera();
void moveTo(coordinate c);
coordinate camPos();

#endif

