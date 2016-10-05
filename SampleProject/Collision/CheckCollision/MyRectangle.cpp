#include "MyRectangle.h"

void MyRectangle::CreateRectangle(Box *box, Point *lastPos)
{
	this->box = box;
	if (lastPos == NULL)
	{
		lastPos = new Point(0, 0);
	}
	else
	{
		this->LastPosition = lastPos;
	}
}

void MyRectangle::CreateRectangle(Point *pos, float width, float height, Point *lastPos)
{
	box->CreateBox(pos, width, height);
	if (lastPos == NULL)
	{
		lastPos = new Point(0, 0);
	}
	else
	{
		this->LastPosition = lastPos;
	}
}

void MyRectangle::SetLastPosition(Point *pos)
{
	LastPosition = pos;
}

void MyRectangle::SetLastPosition(float x, float y)
{
	LastPosition = new Point(x, y);
}

void MyRectangle::UpdatePosition(Point *pos)
{
	box->SetPosition(pos);
}

void MyRectangle::UpdatePosition(float x, float y)
{
	box->SetPosition(x, y);
}

MyRectangle::MyRectangle()
{
	box = new Box();
	LastPosition = new Point();
	TickTime = 0;
}


MyRectangle::~MyRectangle()
{
}
