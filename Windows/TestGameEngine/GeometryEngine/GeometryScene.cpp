#include "GeometryScene.h"

GeometryEngine::GeometryScene::GeometryScene(SceneManager* manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) : mClearColor(clearColor)
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

void GeometryEngine::GeometryScene::DrawItem(Camera * cam, GeometryItem * item)
{
	item->DrawItem(cam->GetProjectionMatrix(), cam->GetViewMatrix());
}

void GeometryEngine::GeometryScene::ApplyLight(Camera * cam)
{
	GBufferTextureInfo gbuff(GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_POSITION,
		GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_NORMAL,
		GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_TEXCOORD, 
		cam->GetGBuffer()->GetTextureSize());

	for (auto iter = mLights.begin(); iter != mLights.end(); ++iter)
	{
	Light* l = (*iter);

	if (l->GetBoundingGeometry() != nullptr)
	{
		l->LightFromBoundignGeometry(cam->GetProjectionMatrix(), cam->GetViewMatrix(), gbuff, cam->GetPosition());
	}
	else
	{
		for (auto it = mItemList.begin(); it != mItemList.end(); ++it)
		{
			GeometryItem* item = (*it);
			Material* mat = item->GetMaterialPtr();
			LightingTransformationData ltd(cam->GetProjectionMatrix(), cam->GetViewMatrix(), item->GetModelMatrix(), item->GetRotation());
			MaterialLightingParameters mlp(mat->GetAmbient(), mat->GetDiffuse(), mat->GetSpecular(), mat->GetShininess());
			
			l->CalculateLighting(item->GetArrayBuffer(), item->GetIndexBuffer(), ltd, mlp, gbuff, cam->GetPosition(), item->GetVertexNumber(), item->GetIndexNumber());
		}
	}
	
	}
}

void GeometryEngine::GeometryScene::gBufferDebug(Camera * cam)
{
	GBuffer* gBuf = cam->GetGBuffer();
	gBuf->BindForReading();
	QVector4D viewport = cam->GetViewportSize();

	GLint HalfWidth = (GLint)(viewport.z() / 2.0f);
	GLint HalfHeight = (GLint)(viewport.w() / 2.0f);

	gBuf->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, viewport.z(), viewport.w(), 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuf->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, viewport.z(), viewport.w(), 0, HalfHeight, HalfWidth, viewport.w(), GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuf->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, viewport.z(), viewport.w(), HalfWidth, HalfHeight, viewport.z(), viewport.w(), GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuf->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, viewport.z(), viewport.w(), HalfWidth, 0, viewport.z(), HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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

void GeometryEngine::GeometryScene::PrepareGeomPass()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void GeometryEngine::GeometryScene::DisableDepth()
{
	// When we get here the depth buffer is already populated and the stencil pass
	// depends on it, but it does not write to it.
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}

void GeometryEngine::GeometryScene::PrepareLightPass()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
}

void GeometryEngine::GeometryScene::GeometryPass()
{
	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		// Clear color and depth buffer
		Camera* cam = (*iter);

		if (cam->GetGBuffer() != nullptr)
		{
			cam->GetGBuffer()->BindForWriting(); // Bind GBuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear GBuffer

			QVector4D viewport = cam->GetViewportSize();
			if (viewport.z() > 0 && viewport.w())
			{
				cam->CalculateProjectionMatrix();

				for (auto it = mItemList.begin(); it != mItemList.end(); ++it)
				{
					DrawItem(cam, (*it));
				}
			}
		}
	}
}

void GeometryEngine::GeometryScene::LightPass()
{
	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		// Clear color and depth buffer
		Camera* cam = (*iter);

		if (cam->GetGBuffer() != nullptr)
		{
			cam->GetGBuffer()->BindForReading(); // Bind GBuffer
			glClear(GL_COLOR_BUFFER_BIT);

			QVector4D viewport = cam->GetViewportSize();
			if (viewport.z() > 0 && viewport.w())
			{
				cam->CalculateProjectionMatrix();
				ApplyLight(cam);
			}
		}
	}

	
}

void GeometryEngine::GeometryScene::Draw()
{
	PrepareGeomPass();
	GeometryPass();
	DisableDepth();

	PrepareLightPass();
	LightPass();


	////////////////////////////////////////////////////////////////
	// GEOMETRY BUFFER DEBUG
	///////////////////////////////////////////////////////////////

	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		Camera* cam = (*iter);
	
		if (cam->GetGBuffer() != nullptr)
		{
			gBufferDebug(cam);
		}
	}*/


}

bool GeometryEngine::GeometryScene::AddItem(GeometryItem* item)
{
	if (mItemList.find(item) != mItemList.end())
		return false;

	mItemList.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::RemoveItem(GeometryItem* item)
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

bool GeometryEngine::GeometryScene::AddLight(Light * item)
{
	if(mLights.find(item) != mLights.end())
		return false;

	mLights.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::RemoveLight(Light * item)
{
	if(mLights.find(item) == mLights.end())
		return false;

	mLights.insert(item);
	return true;
}
