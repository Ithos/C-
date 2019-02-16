#include "Camera.h"

GeometryEngine::Camera::Camera(const QVector4D& viewportSize, bool autoResize, GLdouble zNear, GLdouble zFar, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	WorldItem(pos, rot, scale, parent), mViewportSize(viewportSize), mZNear(zNear), mZFar(zFar), mAutoResize(autoResize), mpGBuffer(nullptr)
{
	mProjection.setToIdentity();
	CalculateModelMatrix();
}

GeometryEngine::Camera::~Camera()
{
	if (mpGBuffer != nullptr)
	{
		delete mpGBuffer;
		mpGBuffer = nullptr;
	}
}

void GeometryEngine::Camera::UpdateModelMatrix(bool updateChildren)
{
	// The transformations on the camera are really the opposite transformations on the world
	mModelMatrix.setToIdentity();
	mModelMatrix.translate(-GetPosition());
	mModelMatrix.rotate(-mRotation);
	mModelMatrix.scale(mScale);

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->UpdateModelMatrix(updateChildren);
		}
	}
}

void GeometryEngine::Camera::ResetCameraBeforeCalculation()
{
	mProjection.setToIdentity();
	glViewport(mViewportSize.x(), mViewportSize.y(), mViewportSize.z(), mViewportSize.w());
}
