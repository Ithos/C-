#include "GlyphDrawer.h"
#include <QtGui\qpainter.h>

using namespace GUI;

GlyphDrawer::GlyphDrawer(std::string filename, QWidget *parent) :
	QMainWindow(parent), file(nullptr)
{
	file = new Reader::TTFFile(filename.c_str());
}

GlyphDrawer::~GlyphDrawer()
{
	delete file;
}

void GUI::GlyphDrawer::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);

	//int i = 2;

	for (int i = 0; i < file->GetLength(); ++i)
	{
		Reader::Glyph* glyph = file->GetGlyphs()[i];

		if (glyph == nullptr || glyph->Type != "simple")
		{
			continue;
		}

		int currentPoint = 0, finalPoints = 0;

		int iX = 0, currentX = 0, currentY = 0, overheadX = 0, overheadY = 0;

		bool first = true;

		while (currentPoint < glyph->Points.size())
		{
			Reader::Point* point = glyph->Points[currentPoint];

			if (currentPoint == 0)
			{
				overheadX = iX + (i > 0 ? file->GetGlyphs()[i - 1]->XMax : 0);
				overheadY = (i > 0 ? file->GetGlyphs()[i - 1]->YMax : 0);

				iX += (glyph->XMax + 10000);
			}

			if (first)
			{
				
				first = false;
			}
			else
			{
				painter.drawLine(currentX/100, currentY/100, point->x/100, point->y/100);
			}

			if (currentPoint == glyph->ContourEnds[finalPoints])
			{
				++finalPoints;
				first = true;
			}

			currentX = point->x + overheadX;
			currentY = point->y + overheadY;

			++currentPoint;

		}
	}

}
