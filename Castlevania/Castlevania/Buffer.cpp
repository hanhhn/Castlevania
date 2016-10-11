#include "Buffer.h"

string Buffer::ConvertToString(int intNumber)
{
	/*Convert << intNumber;
	string temp = Convert.str();
	const char *v = (char*)temp.c_str();
	return (char*)v;*/

	Convert.str("");
	Convert << intNumber;
	string temp = Convert.str();
	return temp;
}

string Buffer::ConverToString(float fltNumver)
{
	Convert.str("");
	Convert << fltNumver;
	string temp = Convert.str();
	return temp;
}

int Buffer::ConverToInt(char *strValue)
{
	Convert.str("");
	return stoi(strValue, nullptr, 0);
}

float Buffer::ConvertToFloat(char *strValue)
{
	Convert.str("");
	std::string::size_type st;
	return stof(strValue, &st);
}

string Buffer::MergeString(string str1, string str2)
{
	return str1 + str2;
}

Buffer* Buffer::operator+(const Buffer& b)
{
	Buffer *bf = new Buffer();
	Convert << this->Value;
	Convert << b.Value;
	Convert >> bf->Value;
	return bf;
}

string Buffer::ConvertToString(float x, float y)
{
	string strX = ConverToString(x);
	string strY = ConverToString(y);
	string msg = "(" + strX + ":" + strY + ")";
	return msg;
}

string Buffer::ConvertToString(Point* point)
{
	string strX = ConverToString(point->X);
	string strY = ConverToString(point->Y);
	string msg = "(" + strX + ":" + strY + ")";
	return msg;
}

Buffer::Buffer(char *value)
{
	Value = value;
}

Buffer::Buffer()
{
	Value = "";
}


Buffer::~Buffer()
{
}
