#pragma once

#ifndef GEOMETRYORTHOGRAPHICCAMERA_H
#define GEOMETRYORTHOGRAPHICCAMERA_H

#include "..\DeferredShadingCamera.h"

namespace GeometryEngine
{
	class OrthographicCamera : public DeferredShadingCamera
	{
	public:
		OrthographicCamera(const QVector4D& viewportSize, const QRect& orthoSize, bool autoResize = true, GLdouble zNear = 0.1, GLdouble zFar = 30.0,
			const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f),
			WorldItem* parent = nullptr);

		~OrthographicCamera();
		virtual void CalculateProjectionMatrix() override;
		virtual void SetOrthoSize(const QRect& orthoSize) { mOrthoSize = QRect(orthoSize); }

	protected:
		QRect mOrthoSize;
	};
}

#endif
