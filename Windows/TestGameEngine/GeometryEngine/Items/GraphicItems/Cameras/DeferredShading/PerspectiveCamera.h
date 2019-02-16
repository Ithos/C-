#pragma once

#ifndef GEOMETRYPERSPECTIVECAMERA_H
#define GEOMETRYPERSPECTIVECAMERA_H

#include "..\DeferredShadingCamera.h"

namespace GeometryEngine
{
	class PerspectiveCamera : public DeferredShadingCamera
	{
	public:
		PerspectiveCamera(const QVector4D& viewportSize, GLdouble fovy = 45.0, GLdouble mAspectRatio = 1.0, bool autoResize = true, GLdouble zNear = 0.1, GLdouble zFar = 30.0,
			const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f),
			WorldItem* parent = nullptr);

		virtual ~PerspectiveCamera();
		void SetAspectRatio(int w, int h) { mAspectRatio = qreal(w) / qreal(h ? h : 1); }
		void SetProjection(GLdouble fovy, GLdouble zNear, GLdouble zFar) { mFoView = fovy; SetBoundaries(zNear, zFar); }
		virtual void SetViewport(const QVector4D& size) override { DeferredShadingCamera::SetViewport(size); SetAspectRatio(size.z(), size.w()); }

		virtual void CalculateProjectionMatrix() override;

	protected:

		GLdouble mFoView;
		GLdouble mAspectRatio;

	};
}

#endif