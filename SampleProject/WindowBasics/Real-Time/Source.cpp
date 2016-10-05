// Beginning Game Programming, Second Edition
// Chapter 4
// GameLoop project
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define APPTITLE "Game Loop"
//function prototypes
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
void DrawBitmap(HDC, char*, int, int);
void Game_Init();
void Game_Run();
void Game_End();


//local variables
HWND global_hwnd;
HDC global_hdc;
//the window event callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	global_hwnd = hWnd;
	global_hdc = GetDC(hWnd);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
//helper function to set up the window properties
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	//fill the struct with info
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE;
	wc.hIconSm = NULL;
	//set up the window with the class info
	return RegisterClassEx(&wc);
}
//helper function to create the window and refresh it
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	//create a new window
	hWnd = CreateWindow(
		APPTITLE, //window class
		APPTITLE, //title bar
		WS_OVERLAPPEDWINDOW, //window style
		CW_USEDEFAULT, //x position of window
		CW_USEDEFAULT, //y position of window
		500, //width of the window
		400, //height of the window
		NULL, //parent window
		NULL, //menu
		hInstance, //application instance
		NULL); //window parameters
			   //was there an error creating the window?
	if (!hWnd)
		return FALSE;
	//display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
//entry point for a Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	int done = 0;
	MSG msg;
	// register the class
	MyRegisterClass(hInstance);
	// initialize application
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;
	//initialize the game
	Game_Init();
	// main message loop
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//look for quit message
			if (msg.message == WM_QUIT)
				done = 1;
			//decode and pass messages on to WndProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//process game loop
		Game_Run();
	}
	//do cleanup
	Game_End();
	return msg.wParam;
}

void Game_Init()
{
	//initialize the game...
	//load bitmaps, meshes, textures, sounds, etc.
	//initialize the random number generator
	srand(time(NULL));
}

void Game_Run()
{
	//this is called once every frame
	//do not include your own loop here!
	int x = 0, y = 0;
	RECT rect;
	GetClientRect(global_hwnd, &rect);
	if (rect.right > 0)
	{
		x = rand() % (rect.right - rect.left);
		y = rand() % (rect.bottom - rect.top);
		DrawBitmap(global_hdc, "c.bmp", x, y);
	}
}

void Game_End()
{ }

void DrawBitmap(HDC hdcDest, char *filename, int x, int y)
{
	HBITMAP image;
	BITMAP bm;
	HDC hdcMem;
	//load the bitmap image
		image = (HBITMAP)LoadImage(0, "c.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//read the bitmap’s properties
	GetObject(image, sizeof(BITMAP), &bm);
	//create a device context for the bitmap
	hdcMem = CreateCompatibleDC(global_hdc);
	SelectObject(hdcMem, image);
	//draw the bitmap to the window (bit block transfer)
	BitBlt(
		global_hdc, //destination device context
		x, y, //x,y location on destination
		bm.bmWidth, bm.bmHeight, //width,height of source bitmap
		hdcMem, //source bitmap device context
		0, 0, //start x,y on source bitmap
		SRCCOPY); //blit method
				  //delete the device context and bitmap
	DeleteDC(hdcMem);
	DeleteObject((HBITMAP)image);
}