#include "DirectionalLight.h"

GeometryEngine::DirectionalLight::DirectionalLight(const QVector3D & direction, GeometryItem* boundingBox, const QVector3D & diffuse, const QVector3D & ambient,
	const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	DeferredShadingLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, parent), mDirection(direction)
{
	initLight();
}

GeometryEngine::DirectionalLight::~DirectionalLight()
{
}

void GeometryEngine::DirectionalLight::initLightShaders()
{
	mVertexShaderKey = LightShaderConstants::DEFERRED_SHADING_VERTEX_SHADER;
	mFragmentShaderKey = LightShaderConstants::DIRECTIONAL_LIGHT_FRAGMENT_SHADER_DS; 
}

void GeometryEngine::DirectionalLight::setProgramParameters(const LightingTransformationData & transformData, const MaterialLightingParameters & matParam,
	const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos)
{
	assert( mpProgram != nullptr && "Shading program not found");
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", /*transformData.ProjectionMatrix * transformData.ViewMatrix */ transformData.ModelMatrix);
		//mpProgram->setUniformValue("modelViewMatrix", transformData.ViewMatrix * transformData.ModelMatrix);
		//mpProgram->setUniformValue("modelMatrix", transformData.ModelMatrix);

		//Set material properties
		//mpProgram->setUniformValue("mMaterial.ambient", matParam.Ambient);
		//mpProgram->setUniformValue("mMaterial.diffuse", matParam.Diffuse);
		//mpProgram->setUniformValue("mMaterial.specular", matParam.Specular);
		//mpProgram->setUniformValue("mMaterial.shininess", matParam.Shininess);

		mpProgram->setUniformValue("mPositionMap", gBuffTexInfo.PositionTexture);
		mpProgram->setUniformValue("mColorMap", gBuffTexInfo.DiffuseTexture);
		mpProgram->setUniformValue("mNormalMap", gBuffTexInfo.NormalTexture);

		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);

		//Set light properties
		mpProgram->setUniformValue("mLight.ambient", this->mColorAmbient);
		mpProgram->setUniformValue("mLight.diffuse", this->mColorDiffuse);
		mpProgram->setUniformValue("mLight.specular", this->mColorSpecular);
		mpProgram->setUniformValue("mLight.direction", this->GetRotation() * this->mDirection);

		mpProgram->setUniformValue("mViewPos", viewPos);
	}
}

void GeometryEngine::DirectionalLight::calculateContribution(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	// Tell OpenGL which VBOs to use
	arrayBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Tell OpenGL programmable pipeline how to locate texture coordinates
	/*int textureCoordinate = mpProgram->attributeLocation("TexCoord");
	mpProgram->enableAttributeArray(textureCoordinate);
	mpProgram->setAttributeBuffer(textureCoordinate, GL_FLOAT, VertexData::TEXTURE_COORDINATES_OFFSET, 3, sizeof(VertexData));*/

	// Tell OpenGL programmable pipeline how to locate vertex position data
	/*int normalVector = mpProgram->attributeLocation("aNormal");
	mpProgram->enableAttributeArray(normalVector);
	mpProgram->setAttributeBuffer(normalVector, GL_FLOAT, VertexData::NORMALS_OFFSET, 3, sizeof(VertexData));*/

	// Draw light
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexNum, GL_UNSIGNED_SHORT, 0);
}
