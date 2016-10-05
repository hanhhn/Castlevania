#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <dinput.h>
#include "Text.h"
#include "MyRectangle.h"
#include "resource.h"
#include "Text.h"
#include "Collision.h"
#include "Buffer.h"


#define APP_CLASS "Collision"
#define APP_TITLE "Check Collision"

#define KEYBOARD_BUFFER_SIZE 1024
#define KEY_DOWN(code) ( IsKeyDown(code) )
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FRAME_RATE 10
#define GRAVITY 1.5f
#define VELOCITY 20


LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddv = NULL;
LPDIRECT3DSURFACE9 back_buffer = NULL;


DWORD _DeltaTime;

LPD3DXSPRITE SpriteHandler = NULL;
LPDIRECTINPUT8 di = NULL;


LPDIRECTINPUTDEVICE8 _keyboard;
BYTE  _keyStates[256];
DIDEVICEOBJECTDATA _KeyEvents[KEYBOARD_BUFFER_SIZE];


LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE);
HWND InitWindow(HINSTANCE, int);
int GameInit(HWND);
void GameRun(HWND);
void GameEnd(HWND);
LPDIRECT3DTEXTURE9 LoadTexture(char*, D3DCOLOR);
void RenderFrame(DWORD);

int InitKeyBoard(HINSTANCE hInstance, HWND hWnd);
int IsKeyDown(int KeyCode);
void OnKeyDown(int KeyCode);
void OneKeyPress(int Delta);
void OnKeyUp(int KeyCode);
void ProcessInput(int Delta);
void ProcessKeyBoard(HWND hWnd);

Collision *collision = new Collision();
MyRectangle *groundRECT = new MyRectangle();
MyRectangle *boxRECT = new MyRectangle();
LPDIRECT3DTEXTURE9 ground = NULL;
LPDIRECT3DTEXTURE9 box = NULL;

int _vpx = 0;
int _vpy = SCREEN_HEIGHT;
D3DXIMAGE_INFO info;

Text *text = new Text();
Buffer *buffer = new Buffer();

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
			_DeltaTime = now - frame_start;
			if (_DeltaTime >= tick_per_frame)
			{
				frame_start = now;
				GameRun(hWnd);
			}
			ProcessKeyBoard(hWnd);
			ProcessInput(_DeltaTime);
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

	result = D3DXCreateSprite(d3ddv, &SpriteHandler);
	if (result != D3D_OK)
		return 0;

	//clear the backbuffer to black
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//create pointer to the back buffer
	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer);

	text->CreateFront(d3ddv);

	ground = LoadTexture("ground.png", D3DCOLOR_XRGB(0, 0, 0));
	box = LoadTexture("box.png", D3DCOLOR_XRGB(0, 0, 0));

	groundRECT->CreateRectangle(new Box(new Point(SCREEN_WIDTH / 2, 200), 400, 50), NULL);
	boxRECT->CreateRectangle(new Box(new Point(100, 0), 100, 100), NULL);
	

	//return okay
	return 1;
}

RECT r;

void GameRun(HWND hwnd)
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

		SetRect(&r, 10, 10, 200, 200);
		if ((collision->CheckCollision(boxRECT, groundRECT)))
		{
			text->DisplayText("Collision", NULL, r);
			//collision->UpdateDynamicPosition(boxRECT, groundRECT);

			SetRect(&r, 10, 52, 200, 200);
			boxRECT->box->CurrentPosition = collision->UpdateDynamicPosition(boxRECT, groundRECT);
			text->DisplayText((char*)(buffer->ConvertToString(boxRECT->box->CurrentPosition)).c_str(), D3DCOLOR_XRGB(255, 255, 255), r);
		}
		else
			text->DisplayText("Not Collision", NULL, r);
		RenderFrame(_DeltaTime);

		SpriteHandler->End();
		//stop rendering
		d3ddv->EndScene();
	}
	//display the back buffer on the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void RenderFrame(DWORD DeltaTime)
{
	SetRect(&r, 10, 38, 200, 200);
	text->DisplayText((char*)("Ground: " + buffer->ConvertToString(groundRECT->box->CurrentPosition)).c_str(), D3DCOLOR_XRGB(255, 255, 255), r);
	
	//ground
	D3DXVECTOR3 groundPosition(groundRECT->box->CurrentPosition->X, groundRECT->box->CurrentPosition->Y, 0);
	D3DXMATRIX groundMatrix;
	D3DXMatrixIdentity(&groundMatrix);
	groundMatrix._22 = -1.0f;
	groundMatrix._41 = -_vpx;
	groundMatrix._42 = _vpy;// SCREEN_HEIGHT;
	D3DXVECTOR4 vp_ground;
	D3DXVec3Transform(&vp_ground, &groundPosition, &groundMatrix);
	D3DXVECTOR3 pos_ground = D3DXVECTOR3(vp_ground.x, vp_ground.y, 0);
	D3DXVECTOR3 bottomCenterGround((float)groundRECT->box->Width / 2, groundRECT->box->Height, 0);
	SpriteHandler->Draw(
		ground,
		NULL,//if null display all
		&bottomCenterGround,
		&pos_ground,
		D3DCOLOR_XRGB(255, 255, 255)	// Hardcode!
	);

	SetRect(&r, 10, 24, 200, 200);
	text->DisplayText(
		(char*)("Box: " + buffer->ConvertToString(boxRECT->box->CurrentPosition) + " LastPos: " + buffer->ConvertToString(boxRECT->LastPosition)).c_str(),
		D3DCOLOR_XRGB(255, 255, 255), r);
	//box
	
	D3DXVECTOR3 boxPosition(boxRECT->box->CurrentPosition->X, boxRECT->box->CurrentPosition->Y, 0);
	D3DXMATRIX boxMatrix;
	D3DXMatrixIdentity(&boxMatrix);
	boxMatrix._22 = -1.0f;
	boxMatrix._41 = -_vpx;
	boxMatrix._42 = _vpy;// SCREEN_HEIGHT;
	D3DXVECTOR4 vp_box;
	D3DXVec3Transform(&vp_box, &boxPosition, &boxMatrix);
	D3DXVECTOR3 pos_box = D3DXVECTOR3(vp_box.x, vp_box.y, 0);
	D3DXVECTOR3 bottomCenterBox((float)boxRECT->box->Width / 2, boxRECT->box->Height, 0);
	SpriteHandler->Draw(
		box,
		NULL,//if null display all
		&bottomCenterBox,
		&pos_box,
		D3DCOLOR_XRGB(255, 255, 255)	// Hardcode!
	);
	
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

void GameEnd(HWND hwnd)
{

	ground->Release();
	box->Release();

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
	return 1;
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
	case DIK_LEFT:
		boxRECT->LastPosition = new Point(boxRECT->box->CurrentPosition);
		boxRECT->box->SetPositionVelocityX(-VELOCITY);
		break;
	case DIK_RIGHT:
		boxRECT->LastPosition = new Point(boxRECT->box->CurrentPosition);
		boxRECT->box->SetPositionVelocityX(VELOCITY);
		break;
	case DIK_UP:
		boxRECT->LastPosition = new Point(boxRECT->box->CurrentPosition);
		boxRECT->box->SetPositionVelocityY(VELOCITY);
		break;
	case DIK_DOWN:
		boxRECT->LastPosition = new Point(boxRECT->box->CurrentPosition);
		boxRECT->box->SetPositionVelocityY(-VELOCITY);
		break;
	}
}

void OneKeyPress(int Delta)
{
	//

	//if (IsKeyDown(DIK_LEFT))
	//{
	//	boxRECT->box->SetPositionVelocityX(-VELOCITY);
	//	float x = boxRECT->box->Position->X;
	//}
	//if (IsKeyDown(DIK_RIGHT))
	//{
	//	boxRECT->box->SetPositionVelocityX(VELOCITY);
	//}
	//if (IsKeyDown(DIK_UP))
	//{
	//	boxRECT->box->SetPositionVelocityY(VELOCITY);
	//}
	//if (IsKeyDown(DIK_DOWN))
	//{
	//	boxRECT->box->SetPositionVelocityY(-VELOCITY);
	//}
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