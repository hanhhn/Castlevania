#pragma once
#include <string.h>
#include <string>
#include <sstream>
#include "Point.h"

using namespace std;

class Buffer
{
	string Value;
	stringstream Convert;
public:
	string ConvertToString(int intNumber);
	string ConverToString(float fltNumver);
	string ConvertToString(float x, float y);
	string ConvertToString(Point* point);

	int ConverToInt(char *srtValue);
	float ConvertToFloat(char *srtValue);
	string MergeString(string str1, string str2);
	Buffer* operator+(const Buffer& b);
	Buffer(char *value);
	Buffer();
	~Buffer();
};

