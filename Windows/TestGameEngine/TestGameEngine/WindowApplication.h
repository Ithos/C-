#pragma once

#ifndef MAINOPENGLWINDOW
#define MAINOPENGLWINDOW


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <GeometryEngine.h>

/// JUST for testing  -- DELETE -- ///

#include <ConfigurationManager.h>
#include <ShaderManager.h>

namespace GeometryEngine {
	class GeometryEngine;
	class Cube;
	class PerspectiveCamera;
	class Light;
	class Sphere;
}

namespace Application
{

	class  CWindowApplication : public QOpenGLWidget, protected QOpenGLFunctions
	{
		Q_OBJECT
	public:
		explicit CWindowApplication(QWidget *parent = 0);
		~CWindowApplication();
	protected:
		void mousePressEvent(QMouseEvent *e) override;
		void mouseReleaseEvent(QMouseEvent *e) override;
		void keyPressEvent(QKeyEvent *e) override;
		void keyReleaseEvent(QKeyEvent *e) override;
		void timerEvent(QTimerEvent *e) override;

		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;

	private:

		void initGeometry(GeometryEngine::GeometryEngine* engine); /// TODO -- this should be done somewhere else

		QBasicTimer timer;
		GeometryEngine::GeometryEngine *mpGeomInstance;
		QVector2D mWindowSize;

		/// Items shouldnt be created here its just for testing purposes
		GeometryEngine::Cube* testCube;
		GeometryEngine::Sphere* testCube2;
		GeometryEngine::PerspectiveCamera* cam;
		GeometryEngine::Light* mainLight;

	};
}

#endif // MAINOPENGLWINDOW