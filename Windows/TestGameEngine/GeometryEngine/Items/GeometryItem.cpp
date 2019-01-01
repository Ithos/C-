#include "GeometryItem.h"

GeometryEngine::GeometryItem::GeometryItem(const QVector3D& pos, const QVector3D & rot,	const QVector3D & scale, WorldItem* parent): 
	WorldItem(pos, rot, scale, parent), mpArrayBuf(nullptr), mpIndexBuf(nullptr), mpProgram(nullptr), mpConfInstance(nullptr)
{
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

	if (mpProgram != nullptr)
	{
		delete(mpProgram);
		mpProgram = nullptr;
	}
}

void GeometryEngine::GeometryItem::DrawItem(const QMatrix4x4& projection)
{
	if (mpProgram != nullptr)
	{
		// Link shader pipeline
		if (!mpProgram->link())
		{
			/// TODO -- log error -- ///
			return;
		}

		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			/// TODO -- log error -- ///
			return;
		}

		setProgramParameters(projection);

		drawGeometry();
	}
}

void GeometryEngine::GeometryItem::initProgram()
{
	for (auto it = mVertexShaderKeyList.begin(); it != mVertexShaderKeyList.end(); ++it)
	{
		// Compile vertex shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, mpShaderManager->GetLoadedShaderContent( (*it) ).c_str()))
		{
			/// TODO -- log error -- ///
			return;
		}
	}

	for (auto it = mFragmentShaderKeyList.begin(); it != mFragmentShaderKeyList.end(); ++it)
	{
		// Compile fragment shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, mpShaderManager->GetLoadedShaderContent((*it) ).c_str()))
		{
			/// TODO -- log error -- ///
			return;
		}
	}

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

	mpProgram = new QOpenGLShaderProgram();

	this->initShaders();
	this->initProgram();
	this->initGeometry();
}

