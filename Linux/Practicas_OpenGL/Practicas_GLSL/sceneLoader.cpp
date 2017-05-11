#include "sceneLoader.h"

meshLoader::meshLoader(const char* filename){
	Assimp::Importer importer;
	const aiScene* scene=importer.ReadFile(filename, aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
	if(!scene || !scene->mRootNode || scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE ){
		std::cout << "The file wasn't succesfully opened " << filename << std::endl;
		return;
	}
	recursiveProcess(scene->mRootNode,scene);//Llamamos a la función recursiva que recorrerá toda la estructura de nodos
}

meshLoader::~meshLoader(){
	for(int i=0;i<meshes.size();i++)delete meshes[i];
}

void meshLoader::draw(unsigned int programId){
	
	for(int i=0;i<meshes.size();i++) meshes[i]->draw(programId);
}

std::vector<meshClass*>& meshLoader::getMeshes(){
	return meshes;
}

unsigned int meshLoader::loadTexture(const char* filename, bool generate){
	SDL_Surface* image=IMG_Load(filename);//Cargamos la imagen mediante SDL
	
	if(image==NULL){//Chequeamos posibles errores
		std::cout<<"no image loaded"<<std::endl;
		return -1;
	}
	
	//Especificamos el formato al que se convertirá la imagen mediante un pixel format (mirar los apartados en la documentación)
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	
	//Convertimos la imagen a un formato independiente del archivo
	SDL_Surface* img2=SDL_ConvertSurface(image,&form,SDL_SWSURFACE);
	 
	 if(img2==NULL){//Chequeamos posibles errores
		std::cout<<"error in image format conversion"<<std::endl;
		return -1;
	}
	
	
	unsigned int id;//Creamos el identificador de la textura
	glGenTextures(1,&id);//Esta funcion nos pide el numero de texturas y la direccion de memoria de los identificadores //El segundo parametro dberia ser la dir de mem a un array con los ids de todas las texturas, en nuestro caso solo necesitamos 1 id
	glBindTexture(GL_TEXTURE_2D,id);//Indicamos el tipo de textura y el identificador
	if(generate) gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,image->w,image->h,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);//Con esto generamos los mipmaps en tiempo de ejecucion, si se quiere mejorar hay que generarlos previamente y cargarlos de forma distinta, mirar documentación
	else glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image->w,image->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);//Cargamos la textura dando diversos parametros y le pasamos el mapa de pixeles
	if(generate)glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//Esto es por si se carga un mipmap
	else glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//Ajustamos el filtro minimo
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Ajustamos el filtro nose si maximo o magnificante o como sea
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	SDL_FreeSurface(image);//Eliminamos el surface pq los datos ya se han copiado
	SDL_FreeSurface(img2);
	return id;//Devolvemos el id de la textura
}

void meshLoader::recursiveProcess(aiNode* node, const aiScene* scene){
	
	//Procesamos cosas
	for(int i=0;i<node->mNumMeshes;i++){//Almacenamos las estructuras contenidas en el nodo y las procesamos
		aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh,scene);
	}
	//Llamada recursiva
	for(int i=0;i<node->mNumChildren;i++){
		recursiveProcess(node->mChildren[i],scene);//Realizamos la misma operación con cada uno de los nodos que salen de este
	}
	
}

void meshLoader::processMesh(aiMesh* mesh,const aiScene* scene){
	std::vector<vertexData> data;
	std::vector<textureData> textures;
	std::vector<unsigned int> indexes;
	
	aiColor4D col;//Aqui obtenemos el color del material
	aiMaterial* mat=scene->mMaterials[mesh->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	vector3d defaultColor(col.r,col.g,col.b);
	
	
	for(int i=0;i<mesh->mNumVertices;i++){
		vertexData tmp;
		vector3d tmpVec;
		
		//Position
		tmpVec.setX(mesh->mVertices[i].x);
		tmpVec.setY(mesh->mVertices[i].y);
		tmpVec.setZ(mesh->mVertices[i].z);
		tmp.position=tmpVec;
		
		//Normals
		tmpVec.setX(mesh->mNormals[i].x);
		tmpVec.setY(mesh->mNormals[i].y);
		tmpVec.setZ(mesh->mNormals[i].z);
		tmp.normal=tmpVec;
		
		//Tangent
		if(mesh->mTangents){//Si hay un puntero al vector
			tmpVec.setX(mesh->mTangents[i].x);
			tmpVec.setY(mesh->mTangents[i].y);
			tmpVec.setZ(mesh->mTangents[i].z);
		}else{
			tmpVec.setX(1.0);
			tmpVec.setY(0);
			tmpVec.setZ(0);
		}
		tmp.tangent=tmpVec;
		
		//Color
		if(mesh->mColors[0]){//Puede haber varios colores, pero solo nos preocupamos del primero
			tmpVec.setX(mesh->mColors[0][i].r);
			tmpVec.setY(mesh->mColors[0][i].g);
			tmpVec.setZ(mesh->mColors[0][i].b);
		}else{
			tmpVec=defaultColor;//Si no hay color damos el del material
		}
		tmp.color=tmpVec;
			
		//Texture
		if(mesh->mTextureCoords[0]){//Puede haber varias texturas, pero solo nos preocupamos de la primera
			tmpVec.setX(mesh->mTextureCoords[0][i].x);
			tmpVec.setY(mesh->mTextureCoords[0][i].y);
		}else{
			tmpVec.setX(0.0);
			tmpVec.setY(0.0);
		}
		tmp.U=tmpVec.getX();//Fijamos las cooredenadas de las texturas
		tmp.V=tmpVec.getY();
		data.push_back(tmp);
	}
		
		//Indices
		for(int i=0;i<mesh->mNumFaces;i++){//Recorremos las caras y almacenamos los indices
			aiFace face=mesh->mFaces[i];
			for(int j=0;j<face.mNumIndices;j++){
				indexes.push_back(face.mIndices[j]);
			}
		}
		
		
		for(int i=0;i<mat->GetTextureCount(aiTextureType_DIFFUSE);i++){//Solo cargamos texturas difusas, pero se podrian cargar otro tipo de texturas
			aiString str;
			mat->GetTexture(aiTextureType_DIFFUSE,i,&str);
			textureData tmp;
			tmp.id=loadTexture(str.C_Str(),true);
			tmp.type=0;
			textures.push_back(tmp);
		}
		
		
		meshes.push_back(new meshClass(&data,&indexes,&textures));
		
}


