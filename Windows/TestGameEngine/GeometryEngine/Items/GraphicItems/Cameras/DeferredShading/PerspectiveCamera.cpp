#include "PerspectiveCamera.h"

GeometryEngine::PerspectiveCamera::PerspectiveCamera(const QVector4D& viewportSize, GLdouble fovy, GLdouble aspectRatio, bool autoResize, GLdouble zNear, GLdouble zFar,
	const QVector3D& pos, const QVector3D & rot, const QVector3D & scale, WorldItem* parent) : DeferredShadingCamera(viewportSize, autoResize, zNear, zFar, pos, rot, scale, parent),
																								mFoView(fovy), mAspectRatio(aspectRatio)
{
}

GeometryEngine::PerspectiveCamera::~PerspectiveCamera()
{
}

void GeometryEngine::PerspectiveCamera::CalculateProjectionMatrix()
{
	ResetCameraBeforeCalculation();
	mProjection.perspective(mFoView, mAspectRatio, mZNear, mZFar);
	ApplyCameraModelMatrix();
}