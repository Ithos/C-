#include "OpenGlQtApplication.h"

bool Application::COpenGlQtApplication::QtOpenGlInit(int argc, char ** argv)
{
	mpApp = new QApplication(argc, argv);
	mpApp->setApplicationName("cube");
	mpApp->setApplicationVersion("0.1");

	mpSurface = new QSurfaceFormat();
	mpSurface->setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(*mpSurface);
	
	return true;
}

int Application::COpenGlQtApplication::run()
{

#ifndef QT_NO_OPENGL
	Application::CWindowApplication widget;
	widget.show();
#else
	QLabel note("OpenGL Support required");
	note.show();
#endif

	return mpApp->exec();
}
