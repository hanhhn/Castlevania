#include "Collision.h"

Collision::DIRECTION Collision::CheckCollision(MyRectangle *dynamic, MyRectangle *statis)
{
	dynamicLeft = dynamic->box->CurrentPosition->X - dynamic->box->Width / 2;
	dynamicRight = dynamic->box->CurrentPosition->X + dynamic->box->Width / 2;
	dynamicBottom = dynamic->box->CurrentPosition->Y;
	dynamicTop = dynamic->box->CurrentPosition->Y + dynamic->box->Height;

	statisLeft = statis->box->CurrentPosition->X - statis->box->Width / 2;
	statisRight = statis->box->CurrentPosition->X + statis->box->Width / 2;
	statisBottom = statis->box->CurrentPosition->Y;
	statisTop = statis->box->CurrentPosition->Y + statis->box->Height;

	//ko va cham
	if (dynamicRight < statisLeft)
	{
		Direction = Collision::None;
		return Collision::None;
	}
	if (dynamicLeft > statisRight)
	{
		Direction = Collision::None;
		return Collision::None;
	}
	if (dynamicBottom > statisTop)
	{
		Direction = Collision::None;
		return Collision::None;
	}
	if (dynamicTop < statisBottom)
	{
		Direction = Collision::None;
		return Collision::None;
	}

	int r = dynamicRight - statisLeft;
	int l = statisRight - dynamicLeft;
	int t = dynamicTop - statisBottom;
	int b = statisTop - dynamicBottom;

	//va cham va xac dinh huong
	if (r <= l && r <= b && r <= t)
	{
		Direction = Collision::Right;
		return	Collision::Right;
	}
		
	if (l <= r && l <= b && l <= t)
	{
		Direction = Collision::Left;
		return Collision::Left;
	}
		
	if (t <= r && t <= l && t <= b)
	{ 
		Direction = Collision::Top;
		return Collision::Top;
	}
	if (b <= r && b <= l && b <= t)
	{
		Direction = Collision::Bottom;
		return Collision::Bottom;
	}
		

}

Point* Collision::UpdateDynamicPosition(MyRectangle *dynamic, MyRectangle *statis)
{
	int Delta = 0;
	int x = 0;
	int y = 0;
	switch (Direction)
	{
	case Collision::None:
		break;
	case Collision::Left:
		Delta = statisRight - dynamicLeft;
		x = dynamic->box->CurrentPosition->X + Delta;
		y = dynamic->box->CurrentPosition->Y;
		break;
	case Collision::Right:
		Delta = dynamicRight - statisLeft;
		x = dynamic->box->CurrentPosition->X - Delta;
		y = dynamic->box->CurrentPosition->Y;
		break;
	case Collision::Top:
		Delta = dynamicTop - statisBottom;
		y = dynamic->box->CurrentPosition->Y - Delta;
		x = dynamic->box->CurrentPosition->X;
		break;
	case Collision::Bottom:
		Delta = statisTop - dynamicBottom;
		y = dynamic->box->CurrentPosition->Y + Delta;
		x = dynamic->box->CurrentPosition->X;
		break;
	}
	return new Point(x, y);
}



MyRectangle* Collision::Update()
{
	return new MyRectangle();
}



Collision::Collision()
{
}


Collision::~Collision()
{
}
