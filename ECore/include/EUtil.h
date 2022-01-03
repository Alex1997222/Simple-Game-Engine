#pragma once
#include "ECommon.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include <Windows.h>
#include <ctime>

namespace E3D
{
	// file stream
	extern std::ofstream *GLogStream;

	// initialize log system
	bool InitLog(const EString& fileName);

	// record message, use inital log to record the log
	void Log(const EChar *string,...);

	// Convert integer to string
	void CloseLog();

	// set random number
	inline EInt RandomInt(EInt from = 0, EInt to = 10)
	{
		EInt ran = rand() % (to - from + 1) + from;
		return ran;
	}

	// convert int to string
	inline EString IntToString(EFloat num)
	{
		std::stringstream ss;
		ss << num;
		EString re;
		ss >> re;
		return re;
	}

	// convert string to int
	inline EInt StringToInit(const EString& str)
	{
		return atoi(str.c_str());
	}

	// convert string to float
	inline EFloat StringToFloat(const EString& str)
	{
		return (EFloat)atoi(str.c_str());
	}

	// Convert to wide string
	inline EWString ToEWstring(const EString &str)
	{
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0);
		WCHAR *tString = new WCHAR[wcsLen + 1];
		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), tString, wcsLen);

		//add '\0' at the end
		tString[wcsLen] = '\0';
		return EWString(tString);
	}

	// judge wether two float are equal
	inline EBool EqualFloat(EFloat l, EFloat r)
	{
		return abs(l - r) <= EPSILON_E6;
	}

	// extract the file name from the path
	inline EString GetNameFromPath(const EString &path)
	{
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");
		if (beg == EString::npos)
			beg = -1;

		return path.substr(beg + 1, end - beg - 1);

	}

	// trim the string
	inline EString Trim(const EString& msg)
	{
		const static EString SPACE_CAHR = "\t\f\v\n\r";
		std::size_t beg = msg.find_first_not_of(SPACE_CAHR);
		
		if (beg > msg.length())
			return EString();

		EString result = msg.substr(beg);
		std::size_t end = result.find_last_not_of(SPACE_CAHR);
		if (end != EString::npos)
			end++;

		return result.substr(0, end);
	}

}