#pragma once
#include "Box.h"
#include <Windows.h>

class MyRectangle
{
protected:
	
public:
	Box *box;
	Point *LastPosition;
	HWND *TickTime;
	void CreateRectangle(Box *box, Point *lastPos);
	void CreateRectangle(Point *pos, float width, float height, Point *lastPos);
	void SetLastPosition(Point *pos);
	void SetLastPosition(float x, float y);
	void UpdatePosition(Point *pos);
	void UpdatePosition(float x, float y);
	MyRectangle();
	~MyRectangle();
};

