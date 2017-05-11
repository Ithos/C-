#include "TTFFile.h"
#include <cassert>

Reader::TTFFile::TTFFile(const char * filename) : pBinaryReader(nullptr), pScalarType(0), pNumTables(0), pSearchRange(0), pEntrySelector(0), pRangeShift(0), pHeadTable(), pLength(0)
{
	pBinaryReader = new TTFReader(filename);
	ReadOffsetTables();
	pHeadTable = readHeadTable(pHeadTable);
	readGlyphs();
	pLength = glyphCount();
}

Reader::TTFFile::~TTFFile()
{
	delete(pBinaryReader);

	for (auto it = pTables.begin(); it != pTables.end(); ++it)
	{
		delete(it->second);
	}
	for (auto it = pGlyphs.begin(); it != pGlyphs.end(); ++it)
	{
		delete(*it);
	}
}

void Reader::TTFFile::ReadOffsetTables()
{
	pScalarType = pBinaryReader->read32ByteUInt();
	pNumTables = pBinaryReader->read16ByteUInt();
	pSearchRange = pBinaryReader->read16ByteUInt();
	pEntrySelector = pBinaryReader->read16ByteUInt();
	pRangeShift = pBinaryReader->read16ByteUInt();

	for (int i = 0; i < pNumTables; ++i)
	{
		std::string tableName = pBinaryReader->readString(4);
		TableData* table = new TableData();
		table->Checksum = pBinaryReader->read32ByteUInt();
		table->Offset = pBinaryReader->read32ByteUInt();
		table->Length = pBinaryReader->read32ByteUInt();;

		pTables.insert(std::pair<std::string, TableData*>(tableName, table));

		if (tableName != "head")
		{
			assert(calculateChecksum(pBinaryReader, table->Offset, table->Length) == table->Checksum, "Table %s checksum failed", i);
		}
	}
}

Reader::HeadTableData& Reader::TTFFile::readHeadTable(HeadTableData& headTable)
{
	assert(pTables["head"] != nullptr, "No head table found.");
	pBinaryReader->seekPosition(pTables["head"]->Offset);

	headTable.Version = pBinaryReader->readFixed();
	headTable.FontRevision = pBinaryReader->readFixed();
	headTable.ChecksumAdjustment = pBinaryReader->read32ByteUInt();
	headTable.MagicNumber = pBinaryReader->read32ByteUInt();
	assert(headTable.MagicNumber == 0x5f0f3cf5, "Missing head table magic number");

	headTable.Flags = pBinaryReader->read16ByteUInt();
	headTable.UnitsPerEm = pBinaryReader->read16ByteUInt();
	headTable.Created = pBinaryReader->readDate(headTable.Created);
	headTable.Modified = pBinaryReader->readDate(headTable.Modified);
	headTable.XMin = pBinaryReader->readFWORD();
	headTable.YMin = pBinaryReader->readFWORD();
	headTable.XMax = pBinaryReader->readFWORD();
	headTable.YMax = pBinaryReader->readFWORD();
	headTable.MacStyle = pBinaryReader->read16ByteUInt();
	headTable.LowestRecPPEM = pBinaryReader->read16ByteUInt();
	headTable.FontDirectionHint = pBinaryReader->read16ByteInt();
	headTable.IndexToLocFormat = pBinaryReader->read16ByteInt();
	headTable.GlyphDataFormat = pBinaryReader->read16ByteInt();

	return headTable;
}

unsigned int Reader::TTFFile::readGlyphOffset(HeadTableData& headTable, unsigned int index, const TableData& glyfTable)
{
	TableData* table = pTables["loca"];
	assert(table != nullptr, "No location table found.");

	
	unsigned int oldFilePos = 0;
	int offset = 0;

	if (headTable.IndexToLocFormat == 1)
	{
		oldFilePos = pBinaryReader->seekPosition(table->Offset + index * 4);
		offset = pBinaryReader->read32ByteUInt();
	}
	else 
	{
		oldFilePos = pBinaryReader->seekPosition(table->Offset + index * 2);
		offset = pBinaryReader->read16ByteUInt() * 2;
	}

	pBinaryReader->seekPosition(oldFilePos);

	return offset + glyfTable.Offset;
}

Reader::Glyph & Reader::TTFFile::readGlyph(HeadTableData & headTable, Reader::Glyph & returnGlyph, unsigned int index, const TableData& table)
{
	unsigned int offset = readGlyphOffset(headTable, index, table);
	//TableData* table = pTables["glyf"];

	if (offset >= table.Offset + table.Length)
	{
		return returnGlyph;
	}

	assert(offset >= table.Offset, "Incongruent glyph offset.");
	assert(offset < table.Offset + table.Length, "Glyph offset out of bounds.");

	pBinaryReader->seekPosition(offset);

	returnGlyph.NumberOfContours = pBinaryReader->read16ByteInt();
	returnGlyph.XMin = pBinaryReader->readFWORD();
	returnGlyph.YMin = pBinaryReader->readFWORD();
	returnGlyph.XMax = pBinaryReader->readFWORD();
	returnGlyph.YMax = pBinaryReader->readFWORD();

	assert(returnGlyph.NumberOfContours >= -1, "Incongruent number of contours.");

	if (returnGlyph.NumberOfContours == -1)
	{
		readCompoundGlyph(headTable, returnGlyph);
	}
	else
	{
		readSimpleGlyph(headTable, returnGlyph);
	}

	return returnGlyph;
}

int Reader::TTFFile::calculateChecksum(TTFReader * reader,unsigned int offset,unsigned int length)
{
	int previousPos = reader->seekPosition(offset);
	unsigned int sum = 0;
	int nlongs = (length + 3) / 4;
	while (nlongs-- > 0) {
		sum += reader->read32ByteUInt();
	}

	reader->seekPosition(previousPos);
	return sum;
}

void Reader::TTFFile::readGlyphs()
{
	/// TODO -- revise -- ///

	Reader::TableData* table = pTables["glyf"];
	assert(table != nullptr, "Table glyphs not found");

	
	pBinaryReader->seekPosition(table->Offset);

	unsigned int index = 0;
	while (pBinaryReader->getReaderPosition() < table->Offset + table->Length)
	{
		Reader::Glyph* glyph = new Reader::Glyph();
		readGlyph(pHeadTable, *glyph, index, *table);
		pGlyphs.push_back(glyph); 

		while (pBinaryReader->getReaderPosition() & 1)
		{
			pBinaryReader->read8ByteUInt();
		}
		++index;
	}
}

Reader::Glyph & Reader::TTFFile::readSimpleGlyph(HeadTableData & headTable, Glyph & returnGlyph)
{
	returnGlyph.Type = "simple";

	for (int i = 0; i < returnGlyph.NumberOfContours; ++i)
	{
		returnGlyph.ContourEnds.emplace_back(pBinaryReader->read16ByteUInt());
	}

	// skip over intructions
	pBinaryReader->seekPosition(pBinaryReader->read16ByteUInt() + pBinaryReader->getReaderPosition());

	if (returnGlyph.ContourEnds.size() == 0)
	{
		return returnGlyph;
	}

	unsigned int numPoints = 0;
	for (int i = 0; i < returnGlyph.ContourEnds.size(); ++i)
	{
		if (returnGlyph.ContourEnds[i] > numPoints)
		{
			numPoints = returnGlyph.ContourEnds[i];
		}
	}

	++numPoints;

	std::vector<unsigned int> flags;

	for (int i = 0; i < numPoints; ++i)
	{
		unsigned int flag = pBinaryReader->read8ByteUInt();

		flags.push_back(flag);
		returnGlyph.Points.push_back(new Point(((flag & (unsigned int)GlyphConstants::ON_CURVE)) > 0)); // ??

		if (flag & (unsigned int)(GlyphConstants::REPEAT))
		{
			unsigned int repeatCount = pBinaryReader->read8ByteUInt();
			assert(repeatCount > 0, "Repeat Count failed");
			i += repeatCount;
			while (repeatCount--)// WARNING: POST DECREMENT
			{
				flags.push_back(flag);
				returnGlyph.Points.push_back(new Point(((flag & (unsigned int)GlyphConstants::ON_CURVE)) > 0));// ??
			}
		}
	}

	readCoords('x', (unsigned int)GlyphConstants::X_IS_BYTE, (unsigned int)GlyphConstants::X_DELTA, returnGlyph.XMin, returnGlyph.XMax, numPoints, flags, returnGlyph);
	readCoords('y', (unsigned int)GlyphConstants::Y_IS_BYTE, (unsigned int)GlyphConstants::Y_DELTA, returnGlyph.YMin, returnGlyph.YMax, numPoints, flags, returnGlyph);

	return returnGlyph;
}

Reader::Glyph & Reader::TTFFile::readCompoundGlyph(HeadTableData & headTable, Glyph & returnGlyph)
{
	returnGlyph.Type = "compound";

	unsigned int flags = (unsigned int)GlyphConstants::MORE_COMPONENTS;

	while (flags & (unsigned int)GlyphConstants::MORE_COMPONENTS)
	{
		int arg1 = 0, arg2 = 0;

		flags = pBinaryReader->read16ByteUInt();
		unsigned int glyphIndex = pBinaryReader->read16ByteUInt();
		int arr[] = { 1,0,0,1,0,0 };

		Component* cmp = new Component(flags, arr);

		if (flags & (unsigned int)GlyphConstants::ARG_1_AND_2_ARE_WORDS)
		{
			arg1 = pBinaryReader->read16ByteInt();
			arg2 = pBinaryReader->read16ByteInt();
		}
		else
		{
			arg1 = pBinaryReader->read8ByteUInt();
			arg2 = pBinaryReader->read8ByteUInt();
		}

		if (flags & (unsigned int)GlyphConstants::ARGS_ARE_XY_VALUES)
		{
			cmp->matrix[4] = arg1;
			cmp->matrix[5] = arg2;
		}
		else
		{
			cmp->DestinationPointIndex = arg1;
			cmp->SourcePointIndex = arg2;
		}

		if (flags & (unsigned int)GlyphConstants::WE_HAVE_A_SCALE)
		{
			cmp->matrix[0] = pBinaryReader->readF2DOT14();
			cmp->matrix[3] = cmp->matrix[0];
		}
		else if (flags & (unsigned int)GlyphConstants::WE_HAVE_AN_X_AND_Y_SCALE)
		{
			cmp->matrix[0] = pBinaryReader->readF2DOT14();
			cmp->matrix[3] = pBinaryReader->readF2DOT14();
		}
		else if (flags & (unsigned int)GlyphConstants::WE_HAVE_A_TWO_BY_TWO)
		{
			cmp->matrix[0] = pBinaryReader->readF2DOT14();
			cmp->matrix[1] = pBinaryReader->readF2DOT14();
			cmp->matrix[2] = pBinaryReader->readF2DOT14();
			cmp->matrix[3] = pBinaryReader->readF2DOT14();
		}
		
		returnGlyph.Components.push_back(cmp);
	}

	return returnGlyph;
}

void Reader::TTFFile::readCoords(char coord, char byteFlag, char deltaFlag, int min, int max, int numPoints, const std::vector<unsigned int>& flags, Glyph& glyph)
{
	int value = 0;
	for (int i = 0; i < numPoints; ++i)
	{
		unsigned int flag = flags[i];
		if (flag & byteFlag)
		{
			if (flag & deltaFlag)
			{
				value += pBinaryReader->read8ByteUInt();
			}
			else
			{
				value -= pBinaryReader->read8ByteUInt();
			}
		}
		else if (~flag & deltaFlag)
		{
			value += pBinaryReader->read16ByteInt();
		}

		if(coord == 'x')
			glyph.Points[i]->x = value;
		else
			glyph.Points[i]->y = value;
	}

}

unsigned int Reader::TTFFile::glyphCount()
{
	assert(pTables["maxp"] != nullptr, "Table maxp not found");
	unsigned int previousPos = pBinaryReader->seekPosition(pTables["maxp"]->Offset + 4);
	unsigned int count = pBinaryReader->read16ByteUInt();
	pBinaryReader->seekPosition(previousPos);
	return count;
}


