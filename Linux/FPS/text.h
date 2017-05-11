#ifndef TEXT_H
#define TEXT_H
#include <cstring>
#include <vector>
#include "objLoader.h"

class text{
	float width, height;
	std::vector<int>characters;
	public:
	text(float,float,std::vector<int>&);
	void drawText(coordinate pos, coordinate rot, const char* tex);
};

#endif
