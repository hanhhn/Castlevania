#pragma once
class Point
{
public:
	float X;
	float Y;
	Point(float x, float y);
	Point *CreatePoint(float x, float y);
	Point(Point *pos);
	Point();
	~Point();
};

