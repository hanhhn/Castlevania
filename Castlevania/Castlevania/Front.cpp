#include "Front.h"

void Front::DisplayText(char *content, D3DCOLOR fontcolor, RECT rect)
{
	if (fontcolor == NULL)
		fontcolor = D3DCOLOR_XRGB(255, 255, 255);
	// Draw some text 
	Font->DrawText(NULL, content, -1, &rect, DT_LEFT | DT_NOCLIP, fontcolor);
}

void Front::CreateFront(LPDIRECT3DDEVICE9 d3ddv)
{
	Result = D3DXCreateFont(d3ddv,     //D3D Device
		14,               //Font height
		0,                //Font width
		FW_NORMAL,        //Font Weight
		1,                //MipLevels
		false,            //Italic
		DEFAULT_CHARSET,  //CharSet
		OUT_DEFAULT_PRECIS, //OutputPrecision
		ANTIALIASED_QUALITY, //Quality
		DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
		"Arial",          //pFacename,
		&Font);         //ppFont
	if (Result != D3D_OK)
		return;
}



Front::Front()
{
	Font = NULL;
}


Front::~Front()
{
	Font->Release();
}
