#pragma once
#include "Box.h"

class BaseObject :
	public Box
{
protected:
	float Vx;
	float Vy;
public:
	
	BaseObject();
	~BaseObject();
};

