#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "ViewPort.h"

class Camera
{
private:
	ViewPort *VPort;
	D3DXMATRIX mt;
protected:
	D3DXVECTOR3 PosOut;
public:
	void SetViewPort(int vpx, int vpy, int vpHeight, int vpWidth);
	void SetViewPort(ViewPort *viewPort);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 WorldTransform(RECT Rect, D3DXVECTOR3 PosIn);
	//void Scale();
	Camera();
	~Camera();
};
