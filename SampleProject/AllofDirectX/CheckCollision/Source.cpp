#pragma once
#include "Rectangle.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dinput.h>
#include <D3dx9core.h>
#include <string>
#include <string.h>
using namespace std;

/*------------------*/
#define APP_CLASS "Sample_Surface"
#define APP_TITLE "Sample Surface - Load image from file"
#define KEYBOARD_BUFFER_SIZE	1024
#define KEY_DOWN(code) ( IsKeyDown(code) )
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FRAME_RATE 30
/*------------------*/


/*------------------*/
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddv = NULL;
LPDIRECT3DSURFACE9 back_buffer = NULL;

LPDIRECT3DTEXTURE9 ground = NULL;
LPDIRECT3DTEXTURE9 box = NULL;

LPD3DXSPRITE SpriteHandler = NULL;
LPDIRECTINPUT8 di = NULL;
/*------------------*/

LPDIRECTINPUTDEVICE8 _keyboard;
BYTE  _keyStates[256];
DIDEVICEOBJECTDATA _KeyEvents[KEYBOARD_BUFFER_SIZE];



MRectangle *groundRECT;
MRectangle *boxRECT;
float Vbox = 0.8f;


ID3DXFont *font = NULL;
RECT font_rect;
const char *fps_string;



/*------------------*/
int _vpx = 0;
int _vpy = SCREEN_HEIGHT;
D3DXIMAGE_INFO info;
/*------------------*/

int InitKeyBoard(HINSTANCE hInstance, HWND hWnd);
int IsKeyDown(int KeyCode);
void OnKeyDown(int KeyCode);
void OneKeyPress(int Delta);
void OnKeyUp(int KeyCode);
void ProcessInput(int Delta);
void ProcessKeyBoard(HWND hWnd);

/*------------------*/
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE);
HWND InitWindow(HINSTANCE, int);
LPDIRECT3DTEXTURE9 LoadTexture(char*, D3DCOLOR);
int GameInit(HWND);
void GameRun(HWND,DWORD);
void GameEnd(HWND);
void RenderFrame(DWORD);
int CheckCollision(MRectangle *first, MRectangle *second);



void DisplayText(RECT rect, char *text)
{
	// Create a colour for the text - in this case blue
	D3DCOLOR fontColor = D3DCOLOR_XRGB(255,255,255);
	// Create a rectangle to indicate where on the screen it should be drawn

	// Draw some text 
	font->DrawText(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, fontColor);
}

/*------------------*/

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
	InitKeyBoard(hInstance, hWnd);
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
			DWORD Deltatime = now - frame_start;
			if (Deltatime >= tick_per_frame)
			{
				frame_start = now;
				GameRun(hWnd, Deltatime);
			}
			ProcessKeyBoard(hWnd);
			ProcessInput(Deltatime);
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
	if (result != D3D_OK)
		return 0;
	//clear the backbuffer to black
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//create pointer to the back buffer
	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer);


	//Texture
	ground = LoadTexture("ground.png", D3DCOLOR_XRGB(0, 0, 0));
	box = LoadTexture("box.png", D3DCOLOR_XRGB(0, 0, 0));


	groundRECT = new MRectangle();
	groundRECT->_Box._Position._X = SCREEN_WIDTH/2;
	groundRECT->_Box._Position._Y = 200;
	groundRECT->_Box._Width = 400;
	groundRECT->_Box._Height = 50;

	boxRECT = new MRectangle();
	boxRECT->_Box._Position._X = 69;
	boxRECT->_Box._Position._Y = 00;
	boxRECT->_Box._Width = 100;
	boxRECT->_Box._Height = 100;

	//return okay
	return 1;
}

int CheckCollision(MRectangle *first, MRectangle *second)
{
	float firstLeft = first->_Box._Position._X - first->_Box._Width / 2;
	float firstRight = first->_Box._Position._X + first->_Box._Width / 2;
	float firstBottom = first->_Box._Position._Y;
	float firstTop = first->_Box._Position._Y + first->_Box._Height;

	float secondLeft = second->_Box._Position._X - second->_Box._Width / 2 ;
	float secondRight = second->_Box._Position._X + second->_Box._Width / 2;
	float secondBottom = second->_Box._Position._Y;
	float secondTop = second->_Box._Position._Y + second->_Box._Height;

	int rl = (firstRight >= secondLeft);
	int lr = (firstLeft <= secondRight);
	int tb = (firstTop >= secondBottom);
	int bt = (firstBottom <= secondTop);

	int collision =  rl && lr && tb && bt ;



	//if (firstRight < secondLeft) collision = 0;
	//if (firstLeft > secondRight) collision = 0;
	//if (firstTop < secondBottom) collision = 0;
	//if (firstBottom > secondTop) collision = 0;

	return collision;
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

void GameRun(HWND hwnd, DWORD DeltaTime)
{

	//make sure the Direct3D device is valid
	if (d3ddv == NULL)
		return;
	//start rendering
	if (d3ddv->BeginScene())
	{

		// Clear back buffer with BLACK
		d3ddv->ColorFill(back_buffer, NULL, D3DCOLOR_XRGB(0, 0, 0));
		SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		RenderFrame(DeltaTime);

		SpriteHandler->End();
		d3ddv->EndScene();
	}
	//display the back buffer on the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void RenderFrame(DWORD DeltaTime)
{
	int collision = CheckCollision(boxRECT,groundRECT);

	RECT textRect;
	textRect.left = 10;
	textRect.right = SCREEN_WIDTH;
	textRect.top = 10;
	textRect.bottom = SCREEN_HEIGHT;

	if (collision)
		DisplayText(textRect,"Colistion");
	else
		DisplayText(textRect,"Not Colistion");
	

	//ground
	D3DXVECTOR3 position1((float)groundRECT->_Box._Position._X, (float)groundRECT->_Box._Position._Y, 0);
	D3DXMATRIX mt1;
	D3DXMatrixIdentity(&mt1);
	mt1._22 = -1.0f;
	mt1._41 = -_vpx;
	mt1._42 = _vpy;// SCREEN_HEIGHT;
	D3DXVECTOR4 vp_pos1;
	D3DXVec3Transform(&vp_pos1, &position1, &mt1);
	D3DXVECTOR3 position_out1 = D3DXVECTOR3(vp_pos1.x, vp_pos1.y, 0);
	D3DXVECTOR3 bottomCenter1((float)groundRECT->_Box._Width/2, (float)groundRECT->_Box._Height, 0);
	SpriteHandler->Draw(
		ground,
		NULL,//if null display all
		&bottomCenter1,
		&position_out1,
		D3DCOLOR_XRGB(255, 255, 255)	// Hardcode!
	);


	/*pos = strcat(pos, std::to_string(vp_pos1.x).c_str());
	pos = strcat(pos, ",");
	pos = strcat(pos, std::to_string(vp_pos1.y).c_str());
	pos = strcat(pos, ",");
	pos = strcat(pos, std::to_string(groundRECT->_Box._Width).c_str());
	pos = strcat(pos, ",");
	pos = strcat(pos, std::to_string(groundRECT->_Box._Height).c_str());
	pos = strcat(pos, ")");*/


	//box
	D3DXVECTOR3 position2((float)boxRECT->_Box._Position._X, (float)boxRECT->_Box._Position._Y, 0);
	D3DXMATRIX mt2;
	D3DXMatrixIdentity(&mt2);
	mt2._22 = -1.0f;
	mt2._41 = -_vpx;
	mt2._42 = _vpy;// SCREEN_HEIGHT;
	D3DXVECTOR4 vp_pos2;
	D3DXVec3Transform(&vp_pos2, &position2, &mt2);
	D3DXVECTOR3 position_out2 = D3DXVECTOR3(vp_pos2.x, vp_pos2.y, 0);
	D3DXVECTOR3 bottomCenter2((float)boxRECT->_Box._Width / 2, (float)boxRECT->_Box._Height, 0);
	SpriteHandler->Draw(
		box,
		NULL,//if null display all
		&bottomCenter2,
		&position_out2,
		D3DCOLOR_XRGB(255, 255, 255)	// Hardcode!
	);
}

void GameEnd(HWND hwnd)
{
	//free the surface
	box->Release();
	ground->Release();
	//release the Direct3D device
	if (d3ddv != NULL)
		d3ddv->Release();
	//release the Direct3D object
	if (d3d != NULL)
		d3d->Release();
	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
	}

	if (di) di->Release();
}

int InitKeyBoard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT _hResult;
	_hResult = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, NULL);
	if (_hResult != D3D_OK)
		return 0;
	_hResult = di->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if (_hResult != D3D_OK)
		return 0;
	_hResult = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	_hResult = _keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size


	_hResult = _keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (_hResult != DI_OK) return 0;

	_hResult = _keyboard->Acquire();
	if (_hResult != DI_OK) return 0;
}

int IsKeyDown(int KeyCode)
{
	return (_keyStates[KeyCode] & 0x80) > 0;
}

void OnKeyDown(int KeyCode)
{
	//

	switch (KeyCode)
	{
	case DIK_SPACE:
		
		break;

	}
}

void OneKeyPress(int Delta)
{
	if (IsKeyDown(DIK_N))
	{
	}
}

void OnKeyUp(int KeyCode)
{

}

void ProcessInput(int Delta)
{
	OneKeyPress(Delta);
}

void ProcessKeyBoard(HWND hWnd)
{
	_keyboard->GetDeviceState(sizeof(_keyStates), _keyStates);
	// trang thay cua phim tai thoi diem chung ta goi ham
	// nếu như bắn đạn thì phải dùng công cụ khác sử dung buffer sác định từng phím


	if (IsKeyDown(DIK_ESCAPE))
	{
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	HRESULT hr = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _KeyEvents, &dwElements, 0);

	// Scan through all data, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = _KeyEvents[i].dwOfs;
		int KeyState = _KeyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			OnKeyDown(KeyCode);
		else
			OnKeyUp(KeyCode);
	}
}