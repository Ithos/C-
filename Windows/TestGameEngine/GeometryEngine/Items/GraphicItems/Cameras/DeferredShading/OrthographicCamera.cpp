#include "OrthographicCamera.h"

GeometryEngine::OrthographicCamera::OrthographicCamera(const QVector4D & viewportSize, const QRect& orthoSize, bool autoResize,
			GLdouble zNear, GLdouble zFar, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale,
			WorldItem* parent) : DeferredShadingCamera(viewportSize, autoResize, zNear, zFar, pos, rot, scale, parent), mOrthoSize(orthoSize)
{
}

GeometryEngine::OrthographicCamera::~OrthographicCamera()
{
}

void GeometryEngine::OrthographicCamera::CalculateProjectionMatrix()
{
	ResetCameraBeforeCalculation();
	mProjection.ortho(mOrthoSize.x(), mOrthoSize.x() + mOrthoSize.width(), mOrthoSize.y() - mOrthoSize.height(), mOrthoSize.y(), mZNear, mZFar);
	ApplyCameraModelMatrix();
}
