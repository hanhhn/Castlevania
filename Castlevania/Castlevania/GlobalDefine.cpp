#pragma once
#include "GlobalDefine.h"

//char *Name = "Castlevania";
HINSTANCE HInstance = NULL;
HWND HWnd = NULL;



int FrameRate = 60;
DWORD DeltaTime = 0;
HRESULT HResult = 0;

LPDIRECT3D9 D3d = NULL;
LPDIRECT3DDEVICE9 D3ddv = NULL;
LPDIRECT3DSURFACE9 Back_buffer = NULL;
LPD3DXSPRITE SpriteHandler = NULL;