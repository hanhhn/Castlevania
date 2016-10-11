#pragma once
#include <d3dx9.h>
#include <string>
#include <Windows.h>
#include <time.h>

using namespace std;

#define APP_TITLE "Castlevania"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


//
//extern char *Name;
extern HINSTANCE HInstance;
extern HWND HWnd;
extern LPDIRECT3DDEVICE9 D3ddv;
extern LPD3DXSPRITE SpriteHandler;
extern int FrameRate;
extern DWORD DeltaTime;
extern HRESULT HResult;
extern LPDIRECT3D9 D3d;
extern LPDIRECT3DDEVICE9 D3ddv;
extern LPDIRECT3DSURFACE9 Back_buffer;

