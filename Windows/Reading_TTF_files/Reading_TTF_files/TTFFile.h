#pragma once

#ifndef _TTFFILE_H
#define _TTFFILE_H

#include "TTFReader.h"
#include <vector>

namespace Reader
{
	enum class GlyphConstants
	{
		// Symple glyph constants
		ON_CURVE = 1,
		X_IS_BYTE = 2,
		Y_IS_BYTE = 4,
		REPEAT = 8,
		X_DELTA = 16,
		Y_DELTA = 32,
		// Compound glyph constants
		ARG_1_AND_2_ARE_WORDS = 1,
		ARGS_ARE_XY_VALUES = 2,
		ROUND_XY_TO_GRID = 4,
		WE_HAVE_A_SCALE = 8,
		// RESERVED              = 16
		MORE_COMPONENTS = 32,
		WE_HAVE_AN_X_AND_Y_SCALE = 64,
		WE_HAVE_A_TWO_BY_TWO = 128,
		WE_HAVE_INSTRUCTIONS = 256,
		USE_MY_METRICS = 512,
		OVERLAP_COMPONENT = 1024
	};

	struct TableData
	{
		unsigned int Checksum;
		unsigned int Offset;
		unsigned int Length;
	};

	struct Point
	{
		unsigned int onCurve;
		unsigned int x;
		unsigned int y;


		Point& operator = (Point& other)
		{
			if (this != &other)
			{
				onCurve = other.onCurve;
				x = other.x;
				y = other.y;
			}

			return *this;
		}

		Point(unsigned int curve)
		{
			onCurve = curve;
			x = 0;
			y = 0;
		}
	};

	struct Component
	{
		unsigned int index;
		int matrix[6];
		int DestinationPointIndex;
		int SourcePointIndex;

		Component& operator =(Component& other)
		{
			if (this != &other)
			{
				index = other.index;
				DestinationPointIndex = other.DestinationPointIndex;
				SourcePointIndex = other.SourcePointIndex;

				matrix[0] = other.matrix[0];
				matrix[1] = other.matrix[1];
				matrix[2] = other.matrix[2];
				matrix[3] = other.matrix[3];
				matrix[4] = other.matrix[4];
				matrix[5] = other.matrix[5];
			}

			return *this;
		}

		Component(unsigned int id, int* arr)
		{
			index = id;
			if ((sizeof(arr) / sizeof(arr[0])) >= 6)
			{
				matrix[0] = arr[0];
				matrix[1] = arr[1];
				matrix[2] = arr[2];
				matrix[3] = arr[3];
				matrix[4] = arr[4];
				matrix[5] = arr[5];
			}
			else {
				matrix[0] = 1;
				matrix[1] = 0;
				matrix[2] = 0;
				matrix[3] = 1;
				matrix[4] = 0;
				matrix[5] = 0;
			}

			DestinationPointIndex = 0;
			SourcePointIndex = 0;
		}

	};


	struct HeadTableData
	{
		HeadTableData& operator = (HeadTableData& other)
		{
			if (this != &other)
			{
				Version = other.Version; 
				FontRevision = other.FontRevision;
				ChecksumAdjustment = other.ChecksumAdjustment; 
				MagicNumber = other.MagicNumber; 
				Flags = other.Flags;
				UnitsPerEm = other.UnitsPerEm; 
				Created = other.Created; 
				Modified = other.Modified; 
				XMin = other.XMin;
				YMin = other.YMin; 
				XMax = other.XMax;
				YMax = other.YMax;
				MacStyle = other.MacStyle;
				LowestRecPPEM = other.LowestRecPPEM;
				FontDirectionHint = other.FontDirectionHint;
				IndexToLocFormat = other.IndexToLocFormat;
				GlyphDataFormat = other.GlyphDataFormat;
				
			}

			return *this;
		}

		HeadTableData() : Version(0), FontRevision(0), ChecksumAdjustment(0), MagicNumber(0), Flags(0), UnitsPerEm(0), Created(), Modified(), XMin(0), YMin(0), XMax(0), YMax(0), MacStyle(0), 
			LowestRecPPEM(0), FontDirectionHint(0), IndexToLocFormat(0), GlyphDataFormat(0)
		{

		}

		float Version;
		float FontRevision;
		unsigned int ChecksumAdjustment;
		unsigned int MagicNumber;
		unsigned int Flags;
		unsigned int UnitsPerEm;
		TTFDate Created;
		TTFDate Modified;
		float XMin;
		float YMin;
		float XMax;
		float YMax;
		unsigned int MacStyle;
		unsigned int LowestRecPPEM;
		int FontDirectionHint;
		int IndexToLocFormat;
		int GlyphDataFormat;
		
	};

	struct Glyph
	{
		Glyph& operator =(Glyph& other)
		{
			if (this != &other)
			{
				NumberOfContours = other.NumberOfContours;
				XMin = other.XMin;
				YMin = other. YMin;
				XMax = other.XMax;
				YMax = other.YMax;
				Type = other.Type;
				Type = other.Type;
				ContourEnds = other.ContourEnds;


				ErasePoints();

				for (int i = 0; i < other.Points.size(); ++i)
				{
					Points.push_back(other.Points[i]);
				}
				
				EraseComponents();

				for (int i = 0; i < other.Components.size(); ++i)
				{
					Components.push_back(other.Components[i]);
				}
			}

			return *this;
		}

		~Glyph()
		{
			ErasePoints();
			EraseComponents();
		}

		void ErasePoints()
		{
			for (int i = 1; i < Points.size();)
			{
				delete(Points[Points.size() - i]);
				Points.pop_back();
			}
		}

		void EraseComponents()
		{
			for (int i = 1; i < Components.size();)
			{
				delete(Components[Components.size() - i]);
				Components.pop_back();
			}
		}

		short int GetWidth() { return (XMax - XMin); }
		short int GetHeight() { return (YMax - YMin); }

		int NumberOfContours;
		short int XMin;
		short int YMin;
		short int XMax;
		short int YMax;
		std::string Type;
		std::vector<unsigned int> ContourEnds;
		std::vector<Point*> Points;
		std::vector<Component*> Components;
	};

	class TTFFile
	{
		public:
			TTFFile(const char* filename);
			~TTFFile();
			unsigned int GetLength() { return pLength; }
			const std::vector<Glyph*>& GetGlyphs(){ return pGlyphs; }
			
		private:
			TTFReader* pBinaryReader;
			unsigned int pScalarType;
			unsigned int pNumTables;
			unsigned int pSearchRange;
			unsigned int pEntrySelector;
			unsigned int pRangeShift;
			std::map<std::string , TableData*> pTables;
			HeadTableData pHeadTable;
			unsigned int pLength;
			std::vector<Glyph*> pGlyphs;

			void ReadOffsetTables();
			int calculateChecksum(TTFReader* reader,unsigned int offset, unsigned int length);
			void readGlyphs();
			Glyph& readSimpleGlyph(HeadTableData& headTable, Glyph& returnGlyph);
			Glyph& readCompoundGlyph(HeadTableData& headTable, Glyph& returnGlyph);
			unsigned int readGlyphOffset(HeadTableData& headTable, unsigned int index, const TableData& glyfTable);
			Glyph& readGlyph(HeadTableData& headTable, Glyph& returnGlyph, unsigned int index, const TableData& table);
			HeadTableData& readHeadTable(HeadTableData& headTable);
			void readCoords(char coord, char byteFlag, char deltaFlag, int min, int max, int numPoints, const std::vector<unsigned int>& flags, Glyph& glyph);
			unsigned int glyphCount();
	};
}

#endif
