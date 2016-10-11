#include "Window.h"

ATOM Window::MyRegisterClass()
{
	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	//fill the struct with info
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)Window::WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APP_TITLE;
	wc.hIconSm = NULL;
	//set up the window with the class info
	return RegisterClassEx(&wc);
}

void Window::CreateMyWindow()
{
	//create a new window

	HWnd = CreateWindow(
		APP_TITLE, //window class
		APP_TITLE, //title bar
		WS_OVERLAPPEDWINDOW, //window style
		CW_USEDEFAULT, //x position of window
		CW_USEDEFAULT, //y position of window
		SCREEN_WIDTH, //width of the window
		SCREEN_HEIGHT, //height of the window
		NULL, //parent window
		NULL, //menu
		HInstance, //application instance
		NULL); //window parameters
			   //was there an error creating the window?
	if (!HWnd)
		return;


	//set position window center screen
	RECT rc;
	GetWindowRect(HWnd, &rc);
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
	SetWindowPos(HWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	//display the window
	ShowWindow(HWnd, SW_SHOWNORMAL);
	//ShowWindow(hWnd, NCmdShow);
	UpdateWindow(HWnd);
}

LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Window::InitWindow()
{
	this->MyRegisterClass();
	this->CreateMyWindow();
}

Window::Window(HINSTANCE hInstance)
{
	HInstance = hInstance;
}

Window::Window()
{
}

Window::~Window()
{
}
