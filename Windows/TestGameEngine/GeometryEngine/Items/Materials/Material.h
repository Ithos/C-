#pragma once

#ifndef GEOMETRYMATERIAL_H
#define GEOMETRYMATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QopenGlFunctions>

#include <list>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "../GeometryItem.h"

namespace GeometryEngine
{
	class GeometryItem;

	struct CustomLightParameters
	{
		QVector3D Ambient;
		QVector3D Diffuse;
		QVector3D Specular;
		QVector3D Position;
		QVector3D Direction;
		QMatrix3x3 Rotation;
		QMatrix4x4 ModelMatrix;
		QVector3D Attenuation;

		const QVector3D& GetAmbient() const { return Ambient; }
		const QVector3D& GetDiffuse() const { return Diffuse; }
		const QVector3D& GetSpecular() const { return Specular; }
		const QVector3D& GetPosition() const { return Position; }
		const QVector3D& GetDirection() const { return Direction; }
		const QVector3D& GetAttenuation() const { return Attenuation; }
		const QMatrix3x3 GetRotation() const { return Rotation; }
		const QMatrix4x4 GetModelMatrix() const { return ModelMatrix; }
	};

	struct MaterialConstants
	{
		static const std::string TEXTURE_MATERIAL_VERTEX_SHADER;
		static const std::string TEXTURE_MATERIAL_FRAGMENT_SHADER;
		static const std::string COLOR_MATERIAL_VERTEX_SHADER;
		static const std::string COLOR_MATERIAL_FRAGMENT_SHADER;
	};

	class Material
	{
	public:
		Material(const QVector3D& ambient = QVector3D(1.0f, 1.0, 1.0f), const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), float shininess = 10.0f, bool isLit = true, bool customLight = false);

		Material(const Material& mat);

		~Material();

		const QVector3D& GetAmbient() const { return mAmbient; }
		const QVector3D& GetDiffuse() const { return mDiffuse; }
		const QVector3D& GetSpecular() const { return mSpecular; }
		float GetShininess() const { return mShininess; }
		bool IsLit() const { return mLit; }
		bool CustomLight() const { return mCustomLight; }

		void SetAmbient(const QVector3D& ambient) { mAmbient.setX(ambient.x()); mAmbient.setY(ambient.y()); mAmbient.setZ(ambient.z()); }
		void SetDiffuse(const QVector3D& diffuse) { mDiffuse.setX(diffuse.x()); mDiffuse.setY(diffuse.y()); mDiffuse.setZ(diffuse.z()); }
		void SetSpecular(const QVector3D& specular) { mSpecular.setX(specular.x()); mSpecular.setY(specular.y()); mSpecular.setZ(specular.z()); }
		void SetShininess(float shininess) { mShininess = shininess; }
		void SetLit(bool lit) { mLit = lit; }
		void SetCustomLight(bool customLight) { mCustomLight = customLight; }

		
		virtual void CustomLighting(std::list<CustomLightParameters* > lights) {}
		virtual void Draw(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber, const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryItem& parent);
		virtual Material* Clone() const = 0;
		


	protected:
		QVector3D mAmbient;
		QVector3D mDiffuse;
		QVector3D mSpecular;
		
		float mShininess;
		bool mLit;
		bool mCustomLight;

		QOpenGLShaderProgram* mpProgram; // Lighting shader
		ShaderFiles::ShaderManager* mpShaderManager;
		Configuration::ConfigurationManager* mpConfInstance;

		std::string mVertexShaderKey;
		std::string mFragmentShaderKey;

		virtual void initMaterial();
		virtual void initShaders() = 0;
		virtual void initProgram();
		virtual void copy(const Material& mat);

		virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryItem& parent) = 0;
		virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) = 0;
	};
}

#endif