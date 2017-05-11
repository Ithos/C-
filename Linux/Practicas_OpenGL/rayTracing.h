#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cmath>
#include "objLoader.h"


#ifndef RAYTRACING_H
#define RAYTRACING_H

bool raysphere(float xc,float yc,float zc,float xd,float yd,float zd,float xs,float ys,float zs,float r, float* dist=NULL, coordinate* point=NULL);
bool rayplane(float xn,float yn,float zn,float xs,float ys,float zs,float xd,float yd,float zd,coordinate p1,coordinate p2,coordinate p3,coordinate p4, float* dist=NULL, coordinate* point=NULL);
float trianglearea(coordinate p1,coordinate p2,coordinate p3);
bool sphereplane(coordinate &sp, coordinate vn,coordinate p1,coordinate p2,coordinate p3,coordinate p4,float r);
bool spheresphere(coordinate &c1,float r1,coordinate c2,float r2);
float pointdistance(coordinate p1,coordinate p2);

#endif
