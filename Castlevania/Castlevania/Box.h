#pragma once
#include "Point.h"

class Box
{
public:
	Point *CurrentPos;
	Point *LastPos;
	Point *NextPos;

	float Width;
	float Height;

public:

	Box();
	~Box();
};

