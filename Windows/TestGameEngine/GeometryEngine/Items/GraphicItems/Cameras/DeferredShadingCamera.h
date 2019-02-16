#pragma once

#ifndef DEFERREDSHADINGCAMERA_H
#define DEFERREDSHADINGCAMERA_H

#include <qapplication.h>
#include <qdesktopwidget.h>
#include "../Camera.h"

namespace GeometryEngine
{
	class DeferredShadingCamera : public Camera
	{
	public:
		DeferredShadingCamera(const QVector4D& viewportSize, bool autoResize = true, GLdouble zNear = 0.1, GLdouble zFar = 30.0, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

		virtual ~DeferredShadingCamera();
		virtual void SetViewport (const QVector4D& size) override;

	protected:
		virtual void init();
	};
}
#endif