#include <iostream>
#include "vector3d.h"

int main(){
	vector3d vec1;
	vector3d vec2(1,2,3);
	vector3d vec3(4,5,6);
	std::cout << vec1 << vec2;
	std::cout << vec2.lenght() << std::endl;
	vec2.normalize();
	std::cout << vec2;
	vector3d vec4=vec3+vec2;
	std::cout << vec4;
	vec4+=vec2;
	std::cout << vec4;
	return 0;
}
