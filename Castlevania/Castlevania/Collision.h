#pragma once
#include "Point.h"
#include "BaseObject.h"

class Collision
{
public:
	enum Direction
	{
		None = 0,
		Left,
		Right,
		Bottom,
		Top
	};
protected:
	Direction Dir;
	float NormalX;
	float NormalY;

	float xInvEntry, xInvExit;
	float yInvEntry, yInvExit;

	float xEntry, xExit;
	float yEntry, yExit;

	float entryTime;
	float exitTime;
public:
	float SweptAABB(BaseObject *Static);
	float AABB(BaseObject *Static);
	virtual Point* UpdateCollision();
	Collision();
	~Collision();
};



