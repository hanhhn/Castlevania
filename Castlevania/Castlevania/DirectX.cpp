#include "DirectX.h"

void DirectX::InitDirectX()
{

	//initialize Direct3D
	D3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (D3d == NULL)
	{
		MessageBox(HWnd, "Error initializing Direct3D", "Error", MB_OK);
		return;
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
	d3dpp.hDeviceWindow = HWnd;

	//create Direct3D device
	D3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		HWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&D3ddv);



	if (D3ddv == NULL)
	{
		MessageBox(HWnd, "Error creating Direct3D device", "Error", MB_OK);
		return;
	}
	//set random number seed
	srand(time(NULL));

	HResult = D3DXCreateSprite(D3ddv, &SpriteHandler);
	if (HResult != D3D_OK)
		return;

	//clear the backbuffer to black
	D3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//create pointer to the back buffer
	D3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &Back_buffer);
}

DirectX::DirectX()
{
}


DirectX::~DirectX()
{
}
