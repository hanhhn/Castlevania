#pragma once
#include "Point.h"
#include <Windows.h>

class Box
{
public:
	Point *CurrentPosition;
	float Width;
	float Height;
	void CreateBox(Point *pos, float width, float height);
	void CreateBox(float x, float y, float width, float height);
	Point* GetPosition();
	RECT GetRectangle();
	void SetPosition(Point *pos);
	void SetPosition(float x, float y);
	void SetPositionVelocityX(float Vx);
	void SetPositionVelocityY(float Vy);
	void SetPositionVelocityXY(float Vx, float Vy);
	Box(Point *pos, float width, float height);
	Box();
	~Box();
};

