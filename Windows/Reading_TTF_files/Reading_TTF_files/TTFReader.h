#pragma once

#ifndef _TTFREADER_H
#define _TTFREADER_H

#include <map>

namespace Reader
{
	struct TTFDate
	{
		TTFDate& operator = (TTFDate& other)
		{
			if (this != &other)
			{
				year = other.year; month = other.month;
				day = other.day; hour = other.hour; minute = other.minute;
				second = other.second;
			}

			return *this;
		}

		static const int monthDays[12];
		TTFDate() {
			year = 1904;
			month = 1;
			day = 1;
			hour = 0;
			minute = 0;
			second = 0;
		}
		unsigned int year;
		unsigned int month;
		unsigned int day;
		unsigned int hour;
		unsigned int minute;
		unsigned int second;

		void addSeconds(long unsigned int seconds)
		{
			long unsigned int minutes = seconds / 60;
			second += seconds % 60;
			long unsigned int hours = minutes / 60;
			minute = minutes % 60;
			long unsigned int days = hours / 24;
			hour = hours % 24;
			unsigned int years = days / 365;
			days = days % 365;
			unsigned int leapYears = 1 + years / 4;

			if (leapYears > days)
			{
				year += (years - 1);
				unsigned int nYearDays = 365;
				if (year % 4 == 0) nYearDays = 366;
				days += (nYearDays - leapYears);
			}
			else
			{
				days -= leapYears;
				year += years;
			}

			for (int i = 0; i < 12; ++i)
			{
				unsigned int monthDays = TTFDate::monthDays[i];
				if (i == 2 && year % 4 == 0) monthDays += 1;
				if (days < monthDays) break;
				days -= monthDays;
				++month;
			}

			day += days;

		}
	};

	class TTFReader
	{
	public:
		TTFReader(const char* ttfPath);
		~TTFReader();
		std::size_t getReaderPosition();
		std::size_t seekPosition(unsigned int newPos);
		int8_t read8ByteInt();
		int16_t read16ByteInt();
		int32_t read32ByteInt();
		unsigned int read8ByteUInt();
		unsigned int read16ByteUInt();
		unsigned int read24ByteUInt();
		unsigned int read32ByteUInt();
		unsigned int readUFWORD();
		int readFWORD();
		float readF2DOT14();
		float readFixed();
		std::string readString(int lenght);
		TTFDate& readDate(TTFDate& date);

	private:
		char* pBuffer;
		int pPos;
		std::streamsize pFileSize;
		unsigned char getCharIncrementPos();
	};
}

#endif