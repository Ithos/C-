#include "WindowApplication.h"

/// TODO remove
#include <Items/Geometries/Cube.h>
#include <Items/GeometryItem.h>
#include <Items/WorldItem.h>
#include <Items/GraphicItems/Camera.h>
#include <Items/GraphicItems/Cameras/PerspectiveCamera.h>
#include <Items/GraphicItems/Cameras/OrthographicCamera.h>

namespace Application
{
	CWindowApplication::CWindowApplication(QWidget *parent) :
		QOpenGLWidget(parent),
		mpGeomInstance(nullptr)
	{
		mWindowSize = QVector2D(this->width(), this->height());
	}

	CWindowApplication::~CWindowApplication()
	{
		// Make sure the context is current when deleting the texture
		// and the buffers.
		makeCurrent();
		GeometryEngine::GeometryEngine::Release();
		doneCurrent();
	}

	void CWindowApplication::mousePressEvent(QMouseEvent * e)
	{
		/// TODO -- get the mouse pressed position and send it to an input manager -- ///
	}

	void CWindowApplication::mouseReleaseEvent(QMouseEvent * e)
	{
		/// TODO -- get the mouse released position and send it to an input manager -- ///
	}

	void CWindowApplication::keyPressEvent(QKeyEvent * e)
	{
		/// TODO -- get key pressed and sent it to an input manager -- ///
	}

	void CWindowApplication::keyReleaseEvent(QKeyEvent * e)
	{
		/// TODO -- get key released and sent it to an input manager -- ///
	}

	void CWindowApplication::timerEvent(QTimerEvent * e)
	{
		/// TODO -- send tick  -- ///

		update();
	}

	void CWindowApplication::initializeGL()
	{
		mpGeomInstance = GeometryEngine::GeometryEngine::GetInstance();
		initGeometry(mpGeomInstance);
		// Use QBasicTimer because its faster than QTimer
		timer.start(12, this);
	}

	void CWindowApplication::resizeGL(int w, int h)
	{
		GeometryEngine::GeometryScene * scene = mpGeomInstance->GetSceneManager()->GetActiveScene();

		if (scene != nullptr)
		{
			scene->ResizeScene(w, h, mWindowSize.x(), mWindowSize.y());
			mWindowSize.setX(w); mWindowSize.setY(h);
		}
	}

	void CWindowApplication::paintGL()
	{
		// Draw cube geometry
		GeometryEngine::GeometryScene * scene = mpGeomInstance->GetSceneManager()->GetActiveScene();

		QVector3D rotation = QVector3D(0.0f, 0.0f, 1.0f);

		testCube->Rotate(testCube->ToModelCoordSystem(rotation));
		testCube2->Rotate(testCube2->ToModelCoordSystem(rotation));
		cam->Rotate(rotation);

		if (scene != nullptr)
		{
			scene->Draw();
		}
	}
	void CWindowApplication::initGeometry(GeometryEngine::GeometryEngine* engine)
	{
		GeometryEngine::GeometryScene* scene = engine->GetSceneManager()->CreateScene();
		/*GeometryEngine::Cube**/ testCube = new GeometryEngine::Cube(QVector3D(-5.0f, 0.0f, -15.0f), QVector3D(30.0f, -30.0f, 0.0f));
		/*GeometryEngine::Cube**/ testCube2 = new GeometryEngine::Cube(QVector3D(5.0f, 0.0f, -15.0f), QVector3D(-30.0f, 30.0f, 0.0f));
		/*GeometryEngine::PerspectiveCamera**/ cam = new GeometryEngine::PerspectiveCamera(QVector4D(0, 0, this->width(), this->height()), 45.0f, 1.0f, true, 0.1f, 30.0f, 
																			QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f));
		//GeometryEngine::OrthographicCamera* cam2 = new GeometryEngine::OrthographicCamera(QVector4D(0, this->height() / 2, this->width()/2, this->height() / 2), QRect(-10, 10, 20, 20));
		scene->AddItem(testCube);
		scene->AddItem(testCube2);
		scene->AddCamera(cam);
		//scene->AddCamera(cam2);
		scene->InitializeGL();
		engine->GetSceneManager()->SetActiveScene(scene);
	}
}
