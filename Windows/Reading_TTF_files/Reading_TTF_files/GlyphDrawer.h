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

		static const float pWidth, pHeight;
		
		int drawGlyph(QPainter& painter, float width, float height, Reader::Glyph* glyph, Reader::Glyph*&  lastGlyph,
			int overheadX = 0, int overheadY = 0, int xOffset = 0, int yOffset = 0, float xScale = 1.0, float yScale = 1.0);
		int drawCompoundGlyph(QPainter& painter, float width, float height, const std::vector<Reader::Glyph*>& glyhpList, Reader::Glyph* glyph,
			Reader::Glyph*&  lastGlyph, int overheadX = 0, int overheadY = 0, Reader::Glyph* acumulationGlyph = nullptr);
	protected:
		void paintEvent(QPaintEvent *e);
	};
}

#endif // MAINWINDOW_H
