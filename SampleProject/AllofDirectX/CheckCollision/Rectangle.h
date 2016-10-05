#pragma once
#include "Box.h"
#include <Windows.h>

class MRectangle
{
public:
	Box _Box;
	Point _LastPosition;
	HWND _TickTime;

	MRectangle();
	~MRectangle();
};

