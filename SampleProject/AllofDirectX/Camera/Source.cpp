#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define APP_CLASS "Sample_Surface"
#define APP_TITLE "Sample Surface - Load image from file"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FRAME_RATE 10

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddv = NULL;

LPDIRECT3DSURFACE9 back_buffer = NULL;
LPDIRECT3DTEXTURE9 texture = NULL;
LPD3DXSPRITE SpriteHandler = NULL;


int _vpx = 0;
int _vpy = SCREEN_HEIGHT;


LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE);
HWND InitWindow(HINSTANCE, int);
LPDIRECT3DTEXTURE9 LoadTexture(char*, D3DCOLOR);
int GameInit(HWND);
void GameRun(HWND);
void GameEnd(HWND);

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
		GameEnd(hWnd);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
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

	result = D3DXCreateSprite(d3ddv, &SpriteHandler);
	if (result != D3D_OK)
		return 0;

	//clear the backbuffer to black
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//create pointer to the back buffer
	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer);

	/*
	//Surface Project
	//create surface
	result = d3ddv->CreateOffscreenPlainSurface(
	SCREEN_WIDTH,							//width of the surface
	SCREEN_HEIGHT,							//height of the surface
	D3DFMT_X8R8G8B8,				//surface format
	D3DPOOL_DEFAULT,				//memory pool to use
	&surface,						//pointer to the surface
	NULL);							//reserved (always NULL)

	*/

	/*
	//Surface From File Project
	//create surface
	result = d3ddv->CreateOffscreenPlainSurface(
	SCREEN_WIDTH, //width of the surface
	SCREEN_HEIGHT, //height of the surface
	D3DFMT_X8R8G8B8, //surface format
	D3DPOOL_DEFAULT, //memory pool to use
	&surface, //pointer to the surface
	NULL); //reserved (always NULL)

	//load surface from file
	result = D3DXLoadSurfaceFromFile(
	surface, //destination surface
	NULL, //destination palette
	NULL, //destination rectangle
	"simon.png", //source filename
	NULL, //source rectangle
	D3DX_DEFAULT, //controls how image is filtered
	D3DCOLOR_XRGB(255, 255, 255), //for transparency (0 for none)
	NULL); //source image info (usually NULL)
	*/


	//Texture
	texture = LoadTexture("simon.png", D3DCOLOR_XRGB(255, 255, 255));

	//return okay
	return 1;
}

LPDIRECT3DTEXTURE9 LoadTexture(char *filename, D3DCOLOR transcolor)
{
	//the texture pointer
	LPDIRECT3DTEXTURE9 texture = NULL;
	//the struct for reading bitmap file info
	D3DXIMAGE_INFO info;
	//standard Windows return value
	HRESULT result;
	//get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK)
		return NULL;
	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx(
		d3ddv, //Direct3D device object
		filename, //bitmap filename
		info.Width, //bitmap image width
		info.Height, //bitmap image height
		1, //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT, //the type of surface (standard)
		D3DFMT_UNKNOWN, //surface format (default)
		D3DPOOL_DEFAULT, //memory class for the texture
		D3DX_DEFAULT, //image filter
		D3DX_DEFAULT, //mip filter
		transcolor, //color key for transparency
		&info, //bitmap file info (from loaded file)
		NULL, //color palette
		&texture); //destination texture

				   //make sure the bitmap texture was loaded correctly
	if (result != D3D_OK)
		return NULL;
	return texture;
}

void GameRun(HWND hwnd)
{

	//make sure the Direct3D device is valid
	if (d3ddv == NULL)
		return;
	//start rendering
	if (d3ddv->BeginScene())
	{

		// Clear back buffer with BLACK
		d3ddv->ColorFill(back_buffer, NULL, D3DCOLOR_XRGB(123, 123, 123));

		D3DXIMAGE_INFO info;
		HRESULT result = D3DXGetImageInfoFromFile("simon.png", &info);

		RECT srect;

		srect.left = 0;
		srect.top = 0;
		srect.right = srect.left + info.Width;
		srect.bottom = srect.top + info.Height;

		SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		D3DXVECTOR3 position_in((float)0, (float)0, 0);

		D3DXMATRIX mt;
		D3DXMatrixIdentity(&mt);

		mt._22 = -1.0f;
		mt._41 = -_vpx;
		mt._42 = _vpy;// SCREEN_HEIGHT;

		D3DXVECTOR4 vp_pos;
		D3DXVec3Transform(&vp_pos, &position_in, &mt);
		D3DXVECTOR3 position_out = D3DXVECTOR3(vp_pos.x, vp_pos.y, 0);

		D3DXVECTOR3 groundCenter((float)info.Width / 2, (float)info.Height, 0);
		
		SpriteHandler->Draw(
			texture,
			NULL,//if null display all
			&groundCenter,
			&position_out,
			D3DCOLOR_XRGB(248, 248, 248)	// Hardcode!
		);
		SpriteHandler->End();



		//stop rendering
		d3ddv->EndScene();
	}
	//display the back buffer on the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void GameEnd(HWND hwnd)
{
	//free the surface
	texture->Release();
	//release the Direct3D device
	if (d3ddv != NULL)
		d3ddv->Release();
	//release the Direct3D object
	if (d3d != NULL)
		d3d->Release();
}

struct ViewPort
{
	int Vpx;
	int Vpy;
} ViewPort;