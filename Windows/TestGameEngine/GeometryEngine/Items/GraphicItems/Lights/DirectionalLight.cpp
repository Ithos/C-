#include "DirectionalLight.h"

GeometryEngine::DirectionalLight::DirectionalLight(const QVector3D & direction, const QVector3D & diffuse, const QVector3D & ambient,
	const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	Light(diffuse, ambient, specular, pos, rot, scale, parent), mDirection(direction)
{
	initLight();
}

GeometryEngine::DirectionalLight::~DirectionalLight()
{
}

void GeometryEngine::DirectionalLight::initLightShaders()
{
	mVertexShaderKey = LightShaderConstants::DIRECTIONAL_LIGHT_VERTEX_SHADER;
	mFragmentShaderKey = LightShaderConstants::DIRECTIONAL_LIGHT_FRAGMENT_SHADER; 
}

void GeometryEngine::DirectionalLight::setProgramParameters(const LightingTransformationData & transformData, const MaterialLightingParameters & matParam, const QVector3D & viewPos)
{
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", transformData.ProjectionMatrix * transformData.ViewMatrix * transformData.ModelMatrix);
		mpProgram->setUniformValue("modelViewMatrix", transformData.ViewMatrix * transformData.ModelMatrix);
		mpProgram->setUniformValue("normalMatrix", transformData.GetNormalMatrix());

		//Set material properties
		mpProgram->setUniformValue("material.ambient", matParam.Ambient);
		mpProgram->setUniformValue("material.diffuse", matParam.Diffuse);
		mpProgram->setUniformValue("material.specular", matParam.Specular);
		mpProgram->setUniformValue("material.shininess", matParam.Shininess);

		//Set light properties
		mpProgram->setUniformValue("light.ambient", this->mColorAmbient);
		mpProgram->setUniformValue("light.diffuse", this->mColorDiffuse);
		mpProgram->setUniformValue("light.specular", this->mColorSpecular);
		mpProgram->setUniformValue("light.direction", this->GetRotation() * this->mDirection);

		mpProgram->setUniformValue("viewPos", viewPos);
	}
}

void GeometryEngine::DirectionalLight::calculateContribution(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum)
{
	// Tell OpenGL which VBOs to use
	arrayBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::GetPositionOffset(), 3, sizeof(VertexData));

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int normalVector = mpProgram->attributeLocation("aNormal");
	mpProgram->enableAttributeArray(normalVector);
	mpProgram->setAttributeBuffer(normalVector, GL_FLOAT, VertexData::GetNormalOffset(), 3, sizeof(VertexData));

	// Draw cube geometry using indices from VBO 1
	glDrawArrays(GL_TRIANGLE_STRIP, 0, totalVertexNum);
}
