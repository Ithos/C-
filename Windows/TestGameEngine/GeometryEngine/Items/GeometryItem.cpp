#include "GeometryItem.h"

GeometryEngine::GeometryItem::GeometryItem(const Material& mat, const QVector3D& pos, const QVector3D & rot,	const QVector3D & scale, WorldItem* parent):
	WorldItem(pos, rot, scale, parent), mpArrayBuf(nullptr), mpIndexBuf(nullptr), mpMaterial(nullptr), mpConfInstance(nullptr), mVertexShaderKey(""), mFragmentShaderKey(""), 
	mTotalVertexNumber(0)
{
	mpMaterial = mat.Clone();
}

GeometryEngine::GeometryItem::~GeometryItem()
{
	if (mpArrayBuf != nullptr)
	{
		mpArrayBuf->destroy();
		delete(mpArrayBuf);
		mpArrayBuf = nullptr;
	}

	if (mpIndexBuf != nullptr)
	{
		mpIndexBuf->destroy();
		delete(mpIndexBuf);
		mpIndexBuf = nullptr;
	}

	if (mpMaterial != nullptr)
	{
		delete(mpMaterial);
		mpMaterial = nullptr;
	}
}

void GeometryEngine::GeometryItem::DrawItem(const QMatrix4x4& projectionView)
{
	if (mpMaterial != nullptr)
	{
		mpMaterial->Draw(mpArrayBuf, mpIndexBuf, mTotalVertexNumber, projectionView, (*this) );
	}
}

void GeometryEngine::GeometryItem::SetMaterial(Material * mat)
{
	if (mpMaterial != nullptr)
	{
		delete(mpMaterial);
	}
	mpMaterial = mat->Clone();
}

void GeometryEngine::GeometryItem::initItem()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpArrayBuf = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	mpIndexBuf = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	// Generate 2 VBOs
	mpArrayBuf->create();
	mpIndexBuf->create();

	this->initGeometry();
}

