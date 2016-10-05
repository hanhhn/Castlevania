#include "Box.h"

void Box::CreateBox(Point *pos, float width, float height)
{
	CurrentPosition = pos;
	Width = width;
	Height = height;
}

void Box::CreateBox(float x, float y, float width, float height)
{
	CurrentPosition = new Point(x, y);
	Width = width;
	Height = height;
}

Point* Box::GetPosition()
{
	return CurrentPosition;
}

void Box::SetPosition(Point *pos)
{
	CurrentPosition = pos;
}

void Box::SetPosition(float x, float y)
{
	Point* p = new Point(x, y);
	CurrentPosition = p;
}


void Box::SetPositionVelocityX(float Vx)
{
	CurrentPosition->X = CurrentPosition->X + Vx;
}

void Box::SetPositionVelocityY(float Vy)
{
	CurrentPosition->Y = CurrentPosition->Y + Vy;
}

void Box::SetPositionVelocityXY(float Vx, float Vy)
{
	CurrentPosition->X = CurrentPosition->X + Vx;
	CurrentPosition->Y = CurrentPosition->Y + Vy;
}

RECT Box::GetRectangle()
{
	RECT value;
	value.left = CurrentPosition->X;
	value.top = CurrentPosition->Y + Height;
	value.bottom = CurrentPosition->Y;
	value.right = CurrentPosition->X + Width;

	return value;
}

Box::Box(Point *pos, float width, float height)
{
	CurrentPosition = pos;
	Width = width;
	Height = height;
}

Box::Box()
{
	CurrentPosition = new Point();
	Width = 0;
	Height = 0;
}


Box::~Box()
{
	delete CurrentPosition;
}
