#pragma once
#ifndef __APPLICATION_OPENGLQTAPPLICATION_H
#define __APPLICATION_OPENGLQTAPPLICATION_H

#include "TGEApplication.h"

#include <QtWidgets/QApplication>

namespace Application
{
	class COpenGlQtApplication
	{
	public:
		COpenGlQtApplication(): mpApp(nullptr), mpSurface(nullptr) {};
		virtual ~COpenGlQtApplication() { delete mpApp; delete mpSurface; };

		bool QtOpenGlInit(int argc, char **argv);
		int run();
	private:
		QApplication* mpApp;
		QSurfaceFormat* mpSurface;
	};
}

#endif