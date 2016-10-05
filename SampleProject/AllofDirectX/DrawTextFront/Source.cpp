#include <windows.h>
#include <d3d9.h>
#include <time.h>
#include <D3dx9core.h>

#define APP_CLASS "Front"
#define APP_TITLE "DirectX with Front"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FRAME_RATE 10

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddv = NULL;
LPDIRECT3DSURFACE9 back_buffer = NULL;
LPDIRECT3DSURFACE9 surface = NULL;

ID3DXFont *font = NULL;
RECT font_rect;
const char *fps_string;



LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE);
HWND InitWindow(HINSTANCE, int);
int GameInit(HWND);
void GameRun(HWND);
void GameEnd(HWND);
void FrontDraw();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// declare variables
	MSG msg;
	// register the class
	MyRegisterClass(hInstance);
	// initialize application
	HWND hWnd = InitWindow(hInstance, nCmdShow);
	//initialize the game
	if (!GameInit(hWnd))
		return 0;
	// main message loop
	int done = 0;
	DWORD frame_start = GetTickCount();
	DWORD tick_per_frame = 1000 / FRAME_RATE;
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
		else
		{
			//process game loop (else prevents running after window is closed)
			DWORD now = GetTickCount();
			if (now - frame_start >= tick_per_frame)
			{
				frame_start = now;
				GameRun(hWnd);
			}
		}

	}
	return msg.wParam;
}

//window event callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		GameEnd(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//initialize
HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{

	HWND hWnd;
	//create a new window

	hWnd = CreateWindow(
		APP_TITLE, //window class
		APP_TITLE, //title bar
		WS_OVERLAPPEDWINDOW, //window style
		CW_USEDEFAULT, //x position of window
		CW_USEDEFAULT, //y position of window
		SCREEN_WIDTH, //width of the window
		SCREEN_HEIGHT, //height of the window
		NULL, //parent window
		NULL, //menu
		hInstance, //application instance
		NULL); //window parameters
			   //was there an error creating the window?
	if (!hWnd)
		return FALSE;


	//set position window center screen
	RECT rc;
	GetWindowRect(hWnd, &rc);
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
	SetWindowPos(hWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);


	//display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
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
	wc.lpszClassName = APP_TITLE;
	wc.hIconSm = NULL;
	//set up the window with the class info
	return RegisterClassEx(&wc);
}

int GameInit(HWND hwnd)
{
	HRESULT result;
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		MessageBox(hwnd, "Error initializing Direct3D", "Error", MB_OK);
		return 0;
	}

	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.hDeviceWindow = hwnd;

	//create Direct3D device
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		MessageBox(hwnd, "Error creating Direct3D device", "Error", MB_OK);
		return 0;
	}
	//set random number seed
	srand(time(NULL));

	//clear the backbuffer to black
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//create pointer to the back buffer
	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer);

	//create surface
	result = d3ddv->CreateOffscreenPlainSurface(
		100,							//width of the surface
		100,							//height of the surface
		D3DFMT_X8R8G8B8,				//surface format
		D3DPOOL_DEFAULT,				//memory pool to use
		&surface,						//pointer to the surface
		NULL);							//reserved (always NULL)
	
	//return okay


	result = D3DXCreateFont(d3ddv,     //D3D Device
		22,               //Font height
		0,                //Font width
		FW_NORMAL,        //Font Weight
		1,                //MipLevels
		false,            //Italic
		DEFAULT_CHARSET,  //CharSet
		OUT_DEFAULT_PRECIS, //OutputPrecision
		ANTIALIASED_QUALITY, //Quality
		DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
		"Arial",          //pFacename,
		&font);         //ppFont
	if (!result)
		return 1;
	return 1;


}

void GameRun(HWND hwnd)
{
	RECT rect;
	int r, g, b;
	//make sure the Direct3D device is valid
	if (d3ddv == NULL)
		return;
	//start rendering
	if (d3ddv->BeginScene())
	{

		// Clear back buffer with BLACK
		d3ddv->ColorFill(back_buffer, NULL, D3DCOLOR_XRGB(123, 123, 123));

		//fill the surface with random color
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		d3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(r, g, b));

		FrontDraw();


		//copy the surface to the backbuffer
		rect.left = rand() % SCREEN_WIDTH / 2;
		rect.right = rect.left + rand() % SCREEN_WIDTH / 2;
		rect.top = rand() % SCREEN_HEIGHT;
		rect.bottom = rect.top + rand() % SCREEN_HEIGHT / 2;
		d3ddv->StretchRect(surface, NULL, back_buffer, &rect, D3DTEXF_NONE);
		//stop rendering
		d3ddv->EndScene();


	}
	//display the back buffer on the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void FrontDraw()
{
	// Create a colour for the text - in this case blue
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 0, 0, 255);
	// Create a rectangle to indicate where on the screen it should be drawn
	RECT rct;
	rct.left = 2;
	rct.right = 780;
	rct.top = 10;
	rct.bottom = rct.top + 20;

	// Draw some text 
	font->DrawText(NULL, "Hello World", -1, &rct, DT_LEFT | DT_NOCLIP, fontColor);
}

void GameEnd(HWND hwnd)
{
	//free the surface
	surface->Release();
	//release the Direct3D device
	if (d3ddv != NULL)
		d3ddv->Release();
	//release the Direct3D object
	if (d3d != NULL)
		d3d->Release();

	if (font) {
		font->Release();
		font = NULL;
	}
}