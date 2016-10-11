#pragma once
#include <Windows.h>
#include "GlobalDefine.h"


class Window
{
private:

	ATOM MyRegisterClass();
	void CreateMyWindow();

protected:
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	void InitWindow();
	Window(HINSTANCE hInstance);
	Window();
	~Window();
};

