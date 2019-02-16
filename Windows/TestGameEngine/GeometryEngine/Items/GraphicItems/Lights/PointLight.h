#pragma once

#ifndef GEOMETRYPOINTLIGHT_H
#define GEOMETRYPOINTLIGHT_H

#include "../DeferredShadingLight.h"

namespace GeometryEngine
{
	class PointLight : public DeferredShadingLight
	{
	public:
		PointLight(const QVector3D& attParam, GeometryItem* boundingBox = nullptr, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

		~PointLight();
	protected:
		QVector3D mAttenuationParameters;
		virtual void initLightShaders();
		virtual void setProgramParameters(const LightingTransformationData & transformData, const MaterialLightingParameters & matParam,
			const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos);
		virtual void calculateContribution(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
		virtual void initLight();
	};
}

#endif