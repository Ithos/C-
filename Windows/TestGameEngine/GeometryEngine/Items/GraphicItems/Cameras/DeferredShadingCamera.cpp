#include "DeferredShadingCamera.h"

GeometryEngine::DeferredShadingCamera::DeferredShadingCamera(const QVector4D& viewportSize, bool autoResize, GLdouble zNear, GLdouble zFar, const QVector3D& pos,
	const QVector3D & rot, const QVector3D & scale, WorldItem* parent) : Camera(viewportSize, autoResize, zNear, zFar, pos, rot, scale, parent)
{
	init();
}

GeometryEngine::DeferredShadingCamera::~DeferredShadingCamera()
{
}

void GeometryEngine::DeferredShadingCamera::SetViewport(const QVector4D & size)
{
	Camera::SetViewport(size);
	mpGBuffer->Resize(mViewportSize.z(), mViewportSize.w());
}

void GeometryEngine::DeferredShadingCamera::init()
{
	mpGBuffer = new GBuffer();
	QRect rec = QApplication::desktop()->screenGeometry();
	mpGBuffer->Init(rec.width(), rec.height());
}
