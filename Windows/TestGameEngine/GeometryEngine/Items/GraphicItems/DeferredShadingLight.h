#pragma once

#ifndef DEFERREDSHADINGLIGHT_H
#define DEFERREDSHADINGLIGHT_H

#include "Light.h"
#include "../GeometryItem.h"

namespace GeometryEngine
{
	class GeometryItem;

	class DeferredShadingLight : public Light
	{
	public:

		DeferredShadingLight(GeometryItem* boundingBox = nullptr, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

		virtual ~DeferredShadingLight();

		virtual void LightFromBoundignGeometry(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos) override;

		virtual WorldItem* const GetBoundingGeometry() override { return mpBoundingBox; }

	protected:
		GeometryItem * mpBoundingBox;

		virtual void ScaleBoundingBox(const QVector3D& attenuation);

	};
}

#endif