#include "TextureMaterial.h"

const std::string GeometryEngine::TextureConstant::ERROR_TEXTURE = "ERROR_TEXTURE";
const std::string GeometryEngine::TextureConstant::TEST_BLUE_CHIP_TEXTURE = "TEST_BLUE_CHIP_TEXTURE";
const std::string GeometryEngine::TextureConstant::TEST_UP_TEXTURE = "TEST_UP_TEXTURE";
const std::string GeometryEngine::TextureConstant::TEST_DOWN_TEXTURE = "TEST_DOWN_TEXTURE";
const std::string GeometryEngine::TextureConstant::TEST_FORWARD_TEXTURE = "TEST_FORWARD_TEXTURE";
const std::string GeometryEngine::TextureConstant::TEST_BACK_TEXTURE = "TEST_BACK_TEXTURE";
const std::string GeometryEngine::TextureConstant::TEST_LEFT_TEXTURE = "TEST_LEFT_TEXTURE";
const std::string GeometryEngine::TextureConstant::TEST_RIGHT_TEXTURE = "TEST_RIGHT_TEXTURE";

GeometryEngine::TextureMaterial::TextureMaterial(const std::string& texDir, const QVector3D & ambient, const QVector3D & diffuse, const QVector3D & specular,
	float shininess, bool isLit, bool customLight) : Material(ambient, diffuse, specular, shininess, isLit, customLight), mpTexDirManager(nullptr)
{
	std::list<TextureParameters*> tmpList;
	TextureParameters tmpParam = TextureParameters(texDir, -1);
	tmpList.push_back(&tmpParam);
	initMaterial(tmpList);
}

GeometryEngine::TextureMaterial::TextureMaterial(const std::list<TextureParameters* >& textureDirs, const QVector3D & ambient, const QVector3D & diffuse,
	const QVector3D & specular, float shininess, bool isLit, bool customLight) : Material(ambient, diffuse, specular, shininess, isLit, customLight), mpTexDirManager(nullptr)
{
	initMaterial( textureDirs );
}

GeometryEngine::TextureMaterial::TextureMaterial(const TextureMaterial & mat)
{
	copy(mat);
}

GeometryEngine::Material * GeometryEngine::TextureMaterial::Clone() const
{
	return new TextureMaterial(*this);
}

GeometryEngine::TextureMaterial::~TextureMaterial()
{
	DeleteAllTextures();
}

void GeometryEngine::TextureMaterial::AddTexture(const TextureParameters& texDir)
{
	mTexturesList.push_back(buildTexture(texDir));
}

void GeometryEngine::TextureMaterial::AddTextures(std::list<TextureParameters*> textureDirs)
{
	for (auto it = textureDirs.begin(); it != textureDirs.end(); ++it)
	{
		AddTexture( *(*it) );
	}
}

void GeometryEngine::TextureMaterial::InsertTexture(const TextureParameters& texDir, int index)
{
	auto iter = mTexturesList.begin();
	std::advance(iter, index);
	mTexturesList.insert(iter, buildTexture(texDir));
}

void GeometryEngine::TextureMaterial::DeleteTexture(int index)
{
	auto iter = mTexturesList.begin();
	std::advance(iter, index);
	delete((*iter));
	(*iter) = nullptr;
	mTexturesList.erase(iter);
}

void GeometryEngine::TextureMaterial::DeleteAllTextures()
{
	for (auto it = mTexturesList.begin(); it != mTexturesList.end(); ++it)
	{
		delete((*it));
		(*it) = nullptr;
	}

	mTexturesList.clear();
}

void GeometryEngine::TextureMaterial::setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryItem & parent)
{
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());

		mpProgram->setUniformValue("texture", TEXTURE_UNIT );
	}
}

void GeometryEngine::TextureMaterial::drawMaterial(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	// Tell OpenGL which VBOs to use
	arrayBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Tell OpenGL programmable pipeline how to locate texture coordinates
	int textureCoordinate = mpProgram->attributeLocation("TexCoord");
	mpProgram->enableAttributeArray(textureCoordinate);
	mpProgram->setAttributeBuffer(textureCoordinate, GL_FLOAT, VertexData::TEXTURE_COORDINATES_OFFSET, 2, sizeof(VertexData));

	// Tell OpenGL programmable pipeline how to locate normals
	int normalVector = mpProgram->attributeLocation("aNormal");
	mpProgram->enableAttributeArray(normalVector);
	mpProgram->setAttributeBuffer(normalVector, GL_FLOAT, VertexData::NORMALS_OFFSET, 3, sizeof(VertexData));

	int vertexCount = 0;

	if (mTexturesList.size() == 1)
	{
		glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);
		return;
	}

	for (int i = 0; i < mTexturesList.size(); ++i)
	{
		auto it = mTexturesList.begin();
		std::advance(it, i);

		if ((*it)->Texture != nullptr)
			(*it)->Texture->bind(TEXTURE_UNIT);

		int vertexNum = (*it)->VertexNumber > 0 ? (*it)->VertexNumber : totalVertexNumber;

		/// Maybe do the same, but with indices?
		glDrawArrays(GL_TRIANGLE_STRIP, vertexCount, vertexNum); /// Draw sets of 3 vertex you can also go for sets of 4 by swapping 3s for 4s in the call to the method

		vertexCount += (*it)->VertexNumber;
	}
}

void GeometryEngine::TextureMaterial::copy(const TextureMaterial & mat)
{
	Material::copy(mat);
	initMaterial(mat.mTexturesList);
}

void GeometryEngine::TextureMaterial::initTextures(const std::list<TextureParameters*  >& textureDirs)
{
	AddTextures(textureDirs);
}

void GeometryEngine::TextureMaterial::initMaterial(const std::list<TextureParameters* >& textureDirs)
{
	Material::initMaterial();
	initTextures(textureDirs);
	mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
}

void GeometryEngine::TextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::MaterialConstants::TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::MaterialConstants::TEXTURE_MATERIAL_FRAGMENT_SHADER;
}

GeometryEngine::TextureParameters * GeometryEngine::TextureMaterial::buildTexture(const TextureParameters & texDir)
{
	TextureParameters* tmp = new TextureParameters(texDir);
	if (tmp->Texture == nullptr) tmp->Build();
	return tmp;
}

void GeometryEngine::TextureParameters::Build()
{
	QImage img = QImage(QString(TextureDir.c_str()));
	
	if (img.isNull())
	{
		img = QImage(QString(mpTexDirManager->GetTextureDir(TextureConstant::ERROR_TEXTURE).c_str()));
	}

	Texture = new QOpenGLTexture(img.mirrored());
}
