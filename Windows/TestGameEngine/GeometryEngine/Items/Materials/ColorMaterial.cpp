#include "ColorMaterial.h"

GeometryEngine::ColorMaterial::ColorMaterial(const QVector3D & emissive, const QVector3D & ambient, const QVector3D & diffuse, 
	const QVector3D & specular, float shininess, bool isLit, bool customLight) : Material(ambient, diffuse, specular, shininess, isLit, customLight), mEmissive(emissive)
{
	initMaterial();
}

GeometryEngine::ColorMaterial::ColorMaterial(const ColorMaterial & mat)
{
	copy(mat);
}

GeometryEngine::ColorMaterial::~ColorMaterial()
{
}

GeometryEngine::Material * GeometryEngine::ColorMaterial::Clone() const
{
	return new ColorMaterial( *this );
}

void GeometryEngine::ColorMaterial::initShaders()
{
	mVertexShaderKey = MaterialConstants::COLOR_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = MaterialConstants::COLOR_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::ColorMaterial::setProgramParameters(const QMatrix4x4 & projectionView, const GeometryItem& parent)
{
	if (mpProgram != nullptr)
	{
		// Set modelview-projection matrix
		mpProgram->setUniformValue("matrix", projectionView * parent.GetModelMatrix());

		// Use texture unit 0 which contains cube.png
		mpProgram->setUniformValue("colAttr", 0);
	}
}

void GeometryEngine::ColorMaterial::drawMaterial(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber)
{
	// Tell OpenGL which VBOs to use
	arrayBuf->bind();
	indexBuf->bind();

	// Offset for position
	quintptr offset = 0;

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	// Offset for color value
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	int color = mpProgram->attributeLocation("colAttr");
	mpProgram->enableAttributeArray(color);
	mpProgram->setAttributeBuffer(color, GL_FLOAT, offset, 3, sizeof(VertexData));

	// Draw cube geometry using indices from VBO 1
	glDrawArrays(GL_TRIANGLE_STRIP, 0, totalVertexNumber);
}

void GeometryEngine::ColorMaterial::copy(const ColorMaterial & mat)
{
	Material::copy(mat);
	this->mEmissive = mat.mEmissive;
	initMaterial();
}
