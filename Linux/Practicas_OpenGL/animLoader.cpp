#include "animLoader.h"

void loadAnimation(std::vector<unsigned int>& frames,std::string filename,unsigned int num,objLoader &loader){
	char tmp[200];
	for(int i=1;i<=num;i++){
		sprintf(tmp,"_%06d.obj",i);//En tmp almacenamos el formato del nombre del archivo
		std::string tmp2(filename+tmp);
		unsigned int id = loader.loadObject(tmp2);
		frames.push_back(id);
	}
}
