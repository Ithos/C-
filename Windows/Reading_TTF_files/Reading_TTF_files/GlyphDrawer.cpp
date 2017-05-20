#include "GlyphDrawer.h"
#include <QtGui\qpainter.h>

using namespace GUI;

GlyphDrawer::GlyphDrawer(std::string filename, QWidget *parent) :
	QMainWindow(parent), file(nullptr)
{
	file = new Reader::TTFFile(filename.c_str());
	showMaximized();
}

GlyphDrawer::~GlyphDrawer()
{
	delete file;
}

void GUI::GlyphDrawer::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);

	int iX = 0, iY = 0;
	int firstX = 0, firstY = 0;
	Reader::Glyph* lastSimpleGlyph = nullptr;


	for (int i = 0; i < file->GetLength(); ++i)
	{
		Reader::Glyph* glyph = file->GetGlyphs()[i];

		if (glyph == nullptr || glyph->Type != "simple")
		{
			continue;
		}

		float xRatio = (32.0/ glyph->GetWidth()), yRatio = (32.0/glyph->GetHeight());

		int currentPoint = 0, finalPoints = 0;

		int currentX = 0, currentY = 0, overheadX = 0, overheadY = 0;

		bool contourStart = true;

		while (currentPoint < glyph->Points.size())
		{
			Reader::Point* point = glyph->Points[currentPoint];

			if (currentPoint == 0)
			{
				iX += ( (i > 0 ? lastSimpleGlyph->GetWidth() * 2 * xRatio : 0));

				if (iX > this->width())
				{
					iX = 0;
					iY -= 100 ;
				}

				overheadX = iX ;
				overheadY = iY - 20;

				

				
			}

			if (contourStart)
			{
				contourStart = false;
				firstX = point->x * xRatio + overheadX;
				firstY = point->y * yRatio + overheadY;

			}
			else
			{
				painter.drawLine(currentX, (glyph->GetHeight()* yRatio - currentY), (overheadX + point->x * xRatio) , (glyph->GetHeight()* yRatio - overheadY - point->y * yRatio) );
			}

			if (currentPoint == glyph->ContourEnds[finalPoints])
			{
				++finalPoints;
				contourStart = true;
				painter.drawLine(point->x * xRatio + overheadX, glyph->GetHeight()* yRatio - (point->y * yRatio + overheadY) , firstX , (glyph->GetHeight()* yRatio - firstY) );
			}

			currentX = point->x * xRatio + overheadX;
			currentY = point->y * yRatio + overheadY;

			++currentPoint;

		}

		lastSimpleGlyph = glyph;
	}

}
