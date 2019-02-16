#include "Light.h"

const std::string GeometryEngine::LightShaderConstants::AMBIENT_LIGHT_VERTEX_SHADER = "AMBIENT_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::LightShaderConstants::AMBIENT_LIGHT_FRAGMENT_SHADER = "AMBIENT_LIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::LightShaderConstants::DIRECTIONAL_LIGHT_VERTEX_SHADER = "DIRECTIONAL_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::LightShaderConstants::DIRECTIONAL_LIGHT_FRAGMENT_SHADER = "DIRECTIONAL_LIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::LightShaderConstants::POINT_LIGHT_VERTEX_SHADER = "POINT_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::LightShaderConstants::POINT_LIGHT_FRAGMENT_SHADER = "POINT_LIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::LightShaderConstants::FLASHLIGHT_VERTEX_SHADER = "FLASHLIGHT_VERTEX_SHADER";
const std::string GeometryEngine::LightShaderConstants::FLASHLIGHT_FRAGMENT_SHADER = "FLASHLIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::LightShaderConstants::DEFERRED_SHADING_VERTEX_SHADER = "DEFERRED_SHADING_VERTEX_SHADER";

const std::string GeometryEngine::LightShaderConstants::AMBIENT_LIGHT_FRAGMENT_SHADER_DS = "AMBIENT_LIGHT_FRAGMENT_SHADER_DS";
const std::string GeometryEngine::LightShaderConstants::DIRECTIONAL_LIGHT_FRAGMENT_SHADER_DS = "DIRECTIONAL_LIGHT_FRAGMENT_SHADER_DS";
const std::string GeometryEngine::LightShaderConstants::POINT_LIGHT_FRAGMENT_SHADER_DS = "POINT_LIGHT_FRAGMENT_SHADER_DS";
const std::string GeometryEngine::LightShaderConstants::FLASHLIGHT_FRAGMENT_SHADER_DS = "FLASHLIGHT_FRAGMENT_SHADER_DS";

GeometryEngine::Light::Light(const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, 
	const QVector3D & rot, const QVector3D & scale, WorldItem * parent) : WorldItem(pos, rot, scale, parent), mColorDiffuse(diffuse), 
	mColorAmbient(ambient), mColorSpecular(specular), mpProgram(nullptr), mVertexShaderKey(""), mFragmentShaderKey("")
{
}

GeometryEngine::Light::~Light()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::Light::CalculateLighting(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, const LightingTransformationData& transformData,
	const MaterialLightingParameters& matParam, const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos, unsigned int totalVertexNum, unsigned int totalIndexNum)
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

		setProgramParameters(transformData, matParam, gBuffTexInfo, viewPos);

		calculateContribution(arrayBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
}

void GeometryEngine::Light::initLight()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpProgram = new QOpenGLShaderProgram();

	this->initLightShaders();
	this->initLightProgram();
}

void GeometryEngine::Light::initLightProgram()
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
