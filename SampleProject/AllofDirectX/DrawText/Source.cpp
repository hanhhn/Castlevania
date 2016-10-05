#include <Windows.h>;
#include <stdlib.h>
#include <time.h>


//application title
#define APPTITLE "Hello World"

//function prototypes (forward declarations)
BOOL InitInstance(HINSTANCE, int);
ATOM MyRegisterClass(HINSTANCE);
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

//the window event callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	char *szHello = "Hello World! Hi Hoang Ngoc Hanh";
	RECT rt;
	int x, y, n;
	COLORREF c;
	switch (message)
	{
	case WM_PAINT:
		//get the dimensions of the window
		GetClientRect(hWnd, &rt);
		//start drawing on device context
		hdc = BeginPaint(hWnd, &ps);
		//draw some text
		DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
		//draw 1000 random pixels
		for (n = 0; n<3000; n++)
		{
			x = rand() % (rt.right - rt.left);
			y = rand() % (rt.bottom - rt.top);
			c = RGB(rand() % 256, rand() % 256, rand() % 256);
			SetPixel(hdc, x, y, c);
		}
		//stop drawing
		EndPaint(hWnd, &ps);
		break;
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
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	// declare variables
	MSG msg;
	// register the class
	MyRegisterClass(hInstance);
	// initialize application
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;
	//set random number seed
	srand(time(NULL));
	//main message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}