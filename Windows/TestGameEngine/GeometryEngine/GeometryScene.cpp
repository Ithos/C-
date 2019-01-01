#include "GeometryScene.h"

GeometryEngine::GeometryScene::GeometryScene(SceneManager* manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) : 
	mFoView(fovy), mZNear(zNear), mZFar(zFar), mClearColor(clearColor)
{
	mpParentManager = manager;
	mpParentManager->AddScene(this);
}

GeometryEngine::GeometryScene::~GeometryScene()
{
}

void GeometryEngine::GeometryScene::InitializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(mClearColor.x(), mClearColor.y(), mClearColor.z(), mClearColor.w());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
}

void GeometryEngine::GeometryScene::ResizeScene(int w, int h, int formerW, int formerH)
{
	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		if ((*iter)->IsAutoResize())
		{
			const QVector4D& viewport = (*iter)->GetViewportSize();
			float wRel = (float)w / (float)formerW;
			float hRel = (float)h / (float)formerH;

			(*iter)->SetViewport(QVector4D(viewport.x() * wRel, viewport.y() * hRel, viewport.z() * wRel, viewport.w() * hRel));
		}
	}
}

void GeometryEngine::GeometryScene::Draw()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		Camera* cam = (*iter);
		QVector4D viewport = cam->GetViewportSize();
		if (viewport.z() > 0 && viewport.w())
		{
			cam->CalculateProjectionMatrix();

			for (auto it = mItemList.begin(); it != mItemList.end(); ++it)
			{
				(*it)->Update( cam->GetProjectionMatrix() );
			}
		}
	}
}

bool GeometryEngine::GeometryScene::AddItem(WorldItem* item)
{
	if (mItemList.find(item) != mItemList.end())
		return false;

	mItemList.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::RemoveItem(WorldItem* item)
{
	if(mItemList.find(item) == mItemList.end())
		return false;

	mItemList.erase(item);
	return true;
}

bool GeometryEngine::GeometryScene::AddCamera(Camera * item)
{
	if (mCameras.find(item) != mCameras.end())
		return false;

	mCameras.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::RemoveCamera(Camera * item)
{
	if (mCameras.find(item) == mCameras.end())
		return false;

	mCameras.erase(item);
	return true;
}
