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

#include <Common\Configuration\ConfigurationManager.h>
#include <Common\ShaderFiles\ShaderManager.h>


class GeometryEngine;

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

		/// TODO -- decide which of this stays and which belongs to lower level classes -- ///

		QBasicTimer timer;
		QOpenGLShaderProgram program;
		GeometryEngine *geometries;

		QMatrix4x4 projection;

		QVector3D rotationAxis;
		qreal angularSpeed;
		QQuaternion rotation;

		void initShaders();

	};
}

#endif // MAINOPENGLWINDOW