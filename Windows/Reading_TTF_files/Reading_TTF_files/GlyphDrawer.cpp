#include "GlyphDrawer.h"
#include <QtGui\qpainter.h>

using namespace GUI;

const float GlyphDrawer::pWidth = 48.0;// 32.0f;
const float GlyphDrawer::pHeight = 48.0;//32.0f;

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

int GUI::GlyphDrawer::drawGlyph(QPainter& painter, float width, float height, Reader::Glyph* glyph, Reader::Glyph*&  lastGlyph,
	int overheadX, int overheadY, int xOffset, int yOffset, float xScale, float yScale)
{

	float xRatio = (width * xScale / glyph->GetWidth()), yRatio = (height * yScale / glyph->GetHeight());

	int currentPoint = 0, finalPoints = 0;

	int currentX = 0, currentY = 0;

	int firstX = 0, firstY = 0;

	int xCoordDisplacement = 0;

	bool contourStart = true;

	while (currentPoint < glyph->Points.size())
	{
		Reader::Point* point = glyph->Points[currentPoint];

		if (currentPoint == 0)
		{
			xCoordDisplacement = ((lastGlyph != nullptr ? lastGlyph->GetWidth() * 2 * xRatio : 0));
		}

		if (contourStart)
		{
			contourStart = false;
			firstX = (point->x + xOffset) * xRatio + overheadX;
			firstY = (point->y + yOffset) * yRatio + overheadY;

		}
		else
		{
			painter.drawLine(currentX, (glyph->GetHeight()* yRatio - currentY), (overheadX + (point->x + xOffset) * xRatio), (glyph->GetHeight()* yRatio - overheadY - (point->y + yOffset) * yRatio));
		}

		if (currentPoint == glyph->ContourEnds[finalPoints])
		{
			++finalPoints;
			contourStart = true;
			painter.drawLine((point->x + xOffset) * xRatio + overheadX, glyph->GetHeight()* yRatio - ((point->y + yOffset) * yRatio + overheadY), firstX, (glyph->GetHeight()* yRatio - firstY));
		}

		currentX = (point->x + xOffset) * xRatio + overheadX;
		currentY = (point->y + yOffset) * yRatio + overheadY;

		++currentPoint;

	}

	lastGlyph = glyph;

	return xCoordDisplacement;

}

int GUI::GlyphDrawer::drawCompoundGlyph(QPainter & painter, float width, float height, const std::vector<Reader::Glyph*>& glyhpList, Reader::Glyph* glyph, Reader::Glyph *& lastGlyph, 
	int overheadX, int overheadY, Reader::Glyph* acumulationGlyph)
{

	int xCoordDisplacement = 0;

	Reader::Glyph* compoundGlyph = acumulationGlyph == nullptr ? new Reader::Glyph() : acumulationGlyph;
	if(acumulationGlyph == nullptr) compoundGlyph->Type = "simple";

	for (auto iter = glyph->Components.begin(); iter != glyph->Components.end(); ++iter)
	{
		Reader::Component* comp = (*iter);
		if (comp == nullptr) continue;

		float horizontalScale = std::sqrt(comp->matrix[0] * comp->matrix[0] + comp->matrix[1] * comp->matrix[1]);
		float verticalScale = std::sqrt(comp->matrix[2] * comp->matrix[2] + comp->matrix[3] * comp->matrix[3]);

		Reader::Glyph* subglyph = glyhpList[comp->index];

		if (subglyph->Type == "compound")
		{
			xCoordDisplacement = drawCompoundGlyph(painter, width, height, glyhpList, glyph, lastGlyph, overheadX, overheadY, acumulationGlyph);
		}
		else
		{

			int compoundCurrentPoints = compoundGlyph->Points.size();

			short width = subglyph->GetWidth(), height = subglyph->GetHeight();

			if (compoundCurrentPoints > 0)
			{
				compoundGlyph->ContourEnds.push_back(compoundCurrentPoints);
			}

			for (auto iter = subglyph->ContourEnds.begin(); iter != subglyph->ContourEnds.end(); ++iter)
			{
				compoundGlyph->ContourEnds.push_back((*iter) + compoundCurrentPoints);
			}

			for (auto iter = subglyph->Points.begin(); iter != subglyph->Points.end(); ++iter)
			{

				Reader::Point point((*iter)->onCurve);
				 
				point.x = (*iter)->x * comp->matrix[0] + (*iter)->y * comp->matrix[1] + width * horizontalScale;
				point.y = (*iter)->y * comp->matrix[3] + (*iter)->x * comp->matrix[2] + height * verticalScale;

				compoundGlyph->Points.push_back( (*iter));
			}


			compoundGlyph->NumberOfContours += subglyph->NumberOfContours;

			int modXMax = subglyph->XMax * comp->matrix[0] + subglyph->YMax * comp->matrix[1] + width * horizontalScale, 
				modXMin = subglyph->XMin * comp->matrix[0] + subglyph->YMin * comp->matrix[1] + width * horizontalScale, 
				modYmax = subglyph->YMax * comp->matrix[3] + subglyph->XMax * comp->matrix[2] + height * verticalScale,
				modYMin = subglyph->YMin * comp->matrix[3] + subglyph->YMin * comp->matrix[2] + height * verticalScale; 

			if (compoundGlyph->XMax < modXMax) compoundGlyph->XMax = modXMax;
			if (compoundGlyph->YMax < modYmax) compoundGlyph->YMax = modYmax;

			if (compoundGlyph->XMin > modXMin) compoundGlyph->XMin = modXMin;
			if (compoundGlyph->YMin > modYMin) compoundGlyph->YMin = modYMin;


		}

	}


	xCoordDisplacement = drawGlyph(painter, width, height, compoundGlyph, lastGlyph, overheadX, overheadY);

	return xCoordDisplacement;
}

void GUI::GlyphDrawer::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);

	int iX = 0, iY = -10;
	int overheadX = 0, overheadY = 0;
	
	Reader::Glyph* lastSimpleGlyph = nullptr;


	for (int i = 0; i < file->GetLength(); ++i)
	{
		Reader::Glyph* glyph = file->GetGlyphs()[i];
		
		//int firstX = 0, firstY = 0;
		

		if (glyph == nullptr )
		{
			continue;
		}
		else if (glyph->Type == "simple")
		{
			iX += drawGlyph(painter, pWidth, pHeight, glyph, lastSimpleGlyph, overheadX, overheadY);
		}
		else if (glyph->Type == "compound")
		{

			iX += drawCompoundGlyph(painter, pWidth, pHeight, file->GetGlyphs(), glyph, lastSimpleGlyph, overheadX, overheadY);
		}


		if (iX > this->width())
		{
			iX = 0;
			iY -= 4 * pHeight;
		}

		overheadX = iX;
		overheadY = iY;
	}

}
