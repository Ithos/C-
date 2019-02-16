#pragma once

#ifndef GEOMETRYLIGHT_H
#define GEOMETRYLIGHT_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "../WorldItem.h"

namespace GeometryEngine
{
	struct MaterialLightingParameters
	{
		const QVector3D& Ambient;
		const QVector3D& Diffuse;
		const QVector3D& Specular;
		float Shininess;

		MaterialLightingParameters(const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular, float shininess):
			Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess) {}
	};

	struct LightingTransformationData
	{
		const QMatrix4x4& ProjectionMatrix;
		const QMatrix4x4& ViewMatrix;
		const QMatrix4x4& ModelMatrix;
		const QQuaternion& ModelRotation;

		LightingTransformationData(const QMatrix4x4& projection, const QMatrix4x4& view, const QMatrix4x4& model, const QQuaternion& rotation): 
			ProjectionMatrix(projection), ViewMatrix(view), ModelMatrix(model), ModelRotation(rotation) {}

		QMatrix3x3 GetNormalMatrix() const { return (ViewMatrix * ModelMatrix).normalMatrix(); }
	};

	struct GBufferTextureInfo
	{
		unsigned int DiffuseTexture;
		unsigned int PositionTexture;
		unsigned int NormalTexture;
		unsigned int TexcoordTexture;
		const QVector2D& TextureSize;

		GBufferTextureInfo(unsigned int diffColorTexture, unsigned int posTexture, unsigned int normalTexture, unsigned int texcoordTexture, const QVector2D& texSize):
			DiffuseTexture(diffColorTexture), PositionTexture(posTexture), NormalTexture(normalTexture), TexcoordTexture(texcoordTexture), TextureSize(texSize) {}
	};

	struct LightShaderConstants
	{
		static const std::string AMBIENT_LIGHT_VERTEX_SHADER;
		static const std::string AMBIENT_LIGHT_FRAGMENT_SHADER;

		static const std::string DIRECTIONAL_LIGHT_VERTEX_SHADER;
		static const std::string DIRECTIONAL_LIGHT_FRAGMENT_SHADER;

		static const std::string POINT_LIGHT_VERTEX_SHADER;
		static const std::string POINT_LIGHT_FRAGMENT_SHADER;

		static const std::string FLASHLIGHT_VERTEX_SHADER;
		static const std::string FLASHLIGHT_FRAGMENT_SHADER;

		static const std::string DEFERRED_SHADING_VERTEX_SHADER;

		static const std::string AMBIENT_LIGHT_FRAGMENT_SHADER_DS;
		static const std::string DIRECTIONAL_LIGHT_FRAGMENT_SHADER_DS;
		static const std::string POINT_LIGHT_FRAGMENT_SHADER_DS;
		static const std::string FLASHLIGHT_FRAGMENT_SHADER_DS;
	};

	class Light : public WorldItem
	{
	public:
		Light(const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

		virtual ~Light();

		virtual void CalculateLighting(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, const LightingTransformationData& transformData, 
			const MaterialLightingParameters& matParam, const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos, unsigned int totalVertexNum, unsigned int totalIndexNum);

		virtual void LightFromBoundignGeometry(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos)
		{ assert(GetBoundingGeometry() != nullptr && "Bounding geometry not found"); } 

		virtual WorldItem* const GetBoundingGeometry() { return nullptr; }
	protected:
		QVector3D mColorDiffuse; // Vec3 color  + float intensity
		QVector3D mColorAmbient;
		QVector3D mColorSpecular;
		QOpenGLShaderProgram* mpProgram; // Lighting shader
		Configuration::ConfigurationManager* mpConfInstance;
		ShaderFiles::ShaderManager* mpShaderManager;
		std::string mVertexShaderKey;
		std::string mFragmentShaderKey;

		virtual void initLight();
		virtual void initLightProgram();
		virtual void initLightShaders() = 0;
		virtual void setProgramParameters(const LightingTransformationData& transformData, const MaterialLightingParameters& matParam, const GBufferTextureInfo& GBuffTexInfo, const QVector3D& viewPos) = 0;
		virtual void calculateContribution(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum) = 0;
	};
}

#endif