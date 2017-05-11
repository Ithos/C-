#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "TTFFile.h"

namespace GUI
{
	class GlyphDrawer : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit GlyphDrawer(std::string filename, QWidget *parent = 0);
		~GlyphDrawer();
	private:
		Reader::TTFFile* file;
	protected:
		void paintEvent(QPaintEvent *e);
	};
}

#endif // MAINWINDOW_H
