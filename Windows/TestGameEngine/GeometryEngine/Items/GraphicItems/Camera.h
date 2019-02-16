#pragma once

#ifndef GEOMETRYCAMERA_H
#define GEOMETRYCAMERA_H

#include "../WorldItem.h"
#include "Render Utils\GBuffer.h"

namespace GeometryEngine
{
	class Camera : public WorldItem
	{
	public:
		// FAQ: Scaling the camera scales the whole coordinate system, be careful with it
		Camera(const QVector4D& viewportSize, bool autoResize = true, GLdouble zNear = 0.1, GLdouble zFar = 30.0, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f),
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
		virtual ~Camera();

		virtual const QMatrix4x4& GetProjectionMatrix() { return mProjection; }
		virtual const QMatrix4x4& GetViewMatrix() { return mModelMatrix; }
		virtual const QMatrix4x4& GetViewProjectionMatrix() { return mViewProjection; }
		virtual void SetViewport(const QVector4D& size) { mViewportSize.setX(size.x()); mViewportSize.setY(size.y()); mViewportSize.setZ(size.z()); mViewportSize.setW(size.w()); }
		void SetBoundaries(GLdouble zNear, GLdouble zFar) { mZNear = zNear; mZFar = zFar; }
		virtual void CalculateProjectionMatrix() = 0;
		virtual void UpdateModelMatrix(bool updateChildren = false) override;
		virtual const QVector4D& GetViewportSize() { return mViewportSize; }
		bool IsAutoResize() { return mAutoResize; }
		virtual GBuffer* GetGBuffer() { return mpGBuffer; }

	protected:
		GLdouble mZNear;
		GLdouble mZFar;
		QVector4D mViewportSize;
		QMatrix4x4 mProjection;
		QMatrix4x4 mViewProjection;
		bool mAutoResize;
		GBuffer* mpGBuffer;
		virtual void ResetCameraBeforeCalculation();
		virtual void ApplyCameraModelMatrix() { mViewProjection = mProjection * mModelMatrix; };
	};

}

#endif