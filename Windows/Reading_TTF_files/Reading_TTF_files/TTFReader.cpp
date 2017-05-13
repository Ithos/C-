#include "TTFReader.h"
#include <iostream>
#include <fstream>
#include <cassert>

namespace Reader
{

	const int TTFDate::monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


	TTFReader::TTFReader(const char * ttfPath)
	{
		std::ifstream ttfFile;
		ttfFile.open(ttfPath, std::ios::in | std::ios::binary);
		if (!ttfFile)
		{
			printf("Error while opening the file.");
		}

		pFileSize = 0;
		pPos = 0;
		ttfFile.seekg(0, std::ios::end); // set the pointer to the end
		pFileSize = ttfFile.tellg();
		ttfFile.seekg(0, std::ios::beg); // set the pointer to the beginning
		pBuffer = new char[pFileSize + 1];
		ttfFile.read(pBuffer, pFileSize);
		pBuffer[pFileSize] = '/0';
		ttfFile.close();

	}

	TTFReader::~TTFReader()
	{
		delete[] pBuffer;
	}

	std::size_t TTFReader::getReaderPosition()
	{
		return pPos;
	}

	std::size_t TTFReader::seekPosition(unsigned int newPos)
	{
		assert(newPos >= 0 && newPos < pFileSize, "Invalid new position.");
		std::size_t formerPos = pPos;
		pPos = newPos;
		return formerPos;
	}

	unsigned char TTFReader::getCharIncrementPos()
	{
		assert(pPos <= pFileSize, "Out of bounds file access.");
		unsigned char retValue = static_cast<unsigned char>(pBuffer[pPos]);
		++pPos;
		return retValue;
	}

	int8_t TTFReader::read8ByteInt()
	{
		return read8ByteUInt();
	}

	int16_t TTFReader::read16ByteInt()
	{
		return read16ByteUInt();
	}

	int32_t TTFReader::read32ByteInt()
	{
		return read32ByteUInt();
	}

	unsigned int TTFReader::read8ByteUInt()
	{
		return getCharIncrementPos();
	}

	unsigned int TTFReader::read16ByteUInt()
	{
		return (getCharIncrementPos() << 8) | getCharIncrementPos();
	}

	unsigned int TTFReader::read24ByteUInt()
	{
		return (getCharIncrementPos() << 16) | (getCharIncrementPos() << 8) | getCharIncrementPos();
	}

	unsigned int TTFReader::read32ByteUInt()
	{
		return (getCharIncrementPos() << 24) | (getCharIncrementPos() << 16) | (getCharIncrementPos() << 8) | getCharIncrementPos();
	}

	unsigned int TTFReader::readUFWORD()
	{
		return read16ByteUInt();
	}

	int TTFReader::readFWORD()
	{
		return read16ByteInt();
	}

	float TTFReader::readF2DOT14()
	{
		return read16ByteInt() / (1 << 14);
	}

	float TTFReader::readFixed()
	{
		return read32ByteInt() / (1 << 16);
	}

	std::string TTFReader::readString(int lenght)
	{
		std::string str;

		for (int i = 0; i < lenght; ++i)
		{
			str += getCharIncrementPos();
		}

		return str;
	}

	TTFDate& TTFReader::readDate(TTFDate& date)
	{
		unsigned long int seconds = (read32ByteUInt() * 0x100000000 + read32ByteUInt());
		
		date.addSeconds(seconds);

		return date;
	}
	
}