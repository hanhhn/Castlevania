#include "Point.h"

Point* Point::CreatePoint(float x, float y)
{
	return new Point(x, y);
}

Point::Point(float x, float y)
{
	this->X = x;
	this->Y = y;
}

Point::Point(Point *pos)
{
	this->X = pos->X;
	this->Y = pos->Y;
}


Point::Point()
{
	this->X = 0;
	this->Y = 0;
}


Point::~Point()
{
}
