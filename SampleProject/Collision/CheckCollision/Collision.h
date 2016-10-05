#pragma once
#include "MyRectangle.h"
#include <cmath>

using namespace std;

class Collision
{
public:
	enum DIRECTION { None, Left, Right, Top, Bottom };
private:
	float dynamicLeft;
	float dynamicRight;
	float dynamicBottom;
	float dynamicTop;

	float statisLeft;
	float statisRight;
	float statisBottom;
	float statisTop;

	DIRECTION Direction;
public:
	DIRECTION CheckCollision(MyRectangle *dynamic, MyRectangle *statis);
	Point* UpdateDynamicPosition(MyRectangle *dynamic, MyRectangle *statis);
	MyRectangle* Update();
	Collision();
	~Collision();
	

};

