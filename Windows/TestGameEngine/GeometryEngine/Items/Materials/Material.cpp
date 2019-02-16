#include "Material.h"

const std::string GeometryEngine::MaterialConstants::TEXTURE_MATERIAL_VERTEX_SHADER = "TEXTURE_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::MaterialConstants::TEXTURE_MATERIAL_FRAGMENT_SHADER = "TEXTURE_MATERIAL_FRAGMENT_SHADER";

const std::string GeometryEngine::MaterialConstants::COLOR_MATERIAL_VERTEX_SHADER = "COLOR_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::MaterialConstants::COLOR_MATERIAL_FRAGMENT_SHADER = "COLOR_MATERIAL_FRAGMENT_SHADER";

GeometryEngine::Material::Material(const QVector3D & ambient, const QVector3D & diffuse, const QVector3D & specular, 
	float shininess, bool isLit , bool customLight) : mpProgram(nullptr),
	mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular), mShininess(shininess), mpShaderManager(nullptr), mpConfInstance(nullptr)
{
}

GeometryEngine::Material::Material(const Material & mat)
{
	copy(mat);
}

GeometryEngine::Material::~Material()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::Material::Draw(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber, const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryItem & parent)
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

		setProgramParameters(projection, view, parent);

		drawMaterial(arrayBuf, indexBuf, totalVertexNumber, totalIndexNumber);
	}
}

void GeometryEngine::Material::initMaterial()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpProgram = new QOpenGLShaderProgram();

	this->initShaders();
	this->initProgram();
}

void GeometryEngine::Material::initProgram()
{
	if (mVertexShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mVertexShaderKey))
		{
			mpShaderManager->LoadShader(mVertexShaderKey);
		}

		// Compile vertex shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, mpShaderManager->GetLoadedShaderContent(mVertexShaderKey).c_str()))
		{
			/// TODO -- log error -- ///
			return;
		}
	}

	if (mFragmentShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mFragmentShaderKey))
		{
			mpShaderManager->LoadShader(mFragmentShaderKey);
		}

		// Compile fragment shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, mpShaderManager->GetLoadedShaderContent(mFragmentShaderKey).c_str()))
		{
			/// TODO -- log error -- ///
			return;
		}
	}
}

void GeometryEngine::Material::copy(const Material & mat)
{
	this->mAmbient = mat.mAmbient;
	this->mDiffuse = mat.mDiffuse;
	this->mSpecular = mat.mSpecular;
	this->mShininess = mat.mShininess;
	this->mLit = mat.mLit;
	this->mCustomLight = mat.mCustomLight;
	this->mpConfInstance = mat.mpConfInstance;
	this->mFragmentShaderKey = mat.mFragmentShaderKey;
	this->mpProgram = nullptr;
	this->mpShaderManager = mat.mpShaderManager;
	this->mVertexShaderKey = mat.mVertexShaderKey;
}
