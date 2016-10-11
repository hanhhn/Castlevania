#pragma once
#include <d3dx9core.h>
#include <d3d9.h>

class Front
{
private:
	ID3DXFont *Font;
	HRESULT Result;
public:
	void CreateFront(LPDIRECT3DDEVICE9 d3ddv);
	void DisplayText(char *content, D3DCOLOR fontcolor, RECT rect);
	Front();
	~Front();
};

