// Beginning Game Programming, Second Edition
// Chapter 6
// Create_Surface program
//header files to include
#include <d3d9.h>
#include <time.h>
#include <Windows.h>
#include <d3dx9.h>
//application title
#define APPTITLE "Load_Bitmap"

//macros to read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
//screen resolution
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
//forward declarations
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE);
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);
//Direct3D objects
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPDIRECT3DSURFACE9 surface = NULL;
//window event callback function

int Game_Init(HWND hwnd)
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
	d3dpp.Windowed = FALSE;
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
		&d3ddev);
	if (d3ddev == NULL)
	{
		MessageBox(hwnd, "Error creating Direct3D device", "Error", MB_OK);
		return 0;
	}
	//set random number seed
	srand(time(NULL));
	//clear the backbuffer to black
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//create pointer to the back buffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	//create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		640, //width of the surface
		480, //height of the surface
		D3DFMT_X8R8G8B8, //surface format
		D3DPOOL_DEFAULT, //memory pool to use
		&surface, //pointer to the surface
		NULL); //reserved (always NULL)
	if (result != D3D_OK)
		return 1;
	//load surface from file
	result = D3DXLoadSurfaceFromFile(
		surface, //destination surface
		NULL, //destination palette
		NULL, //destination rectangle
		"legotron.bmp", //source filename
		NULL, //source rectangle
		D3DX_DEFAULT, //controls how image is filtered
		0, //for transparency (0 for none)
		NULL); //source image info (usually NULL)
			   //make sure file was loaded okay
	if (result != D3D_OK)
		return 1;
	//draw surface to the backbuffer
	d3ddev->StretchRect(surface, NULL, backbuffer, NULL, D3DTEXF_NONE);
	//return okay
	return 1;
}
void Game_Run(HWND hwnd)
{
	//make sure the Direct3D device is valid
	if (d3ddev == NULL)
		return;
	//start rendering
	if (d3ddev->BeginScene())
	{
		//create pointer to the back buffer
		d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
		//draw surface to the backbuffer
		d3ddev->StretchRect(surface, NULL, backbuffer, NULL, D3DTEXF_NONE);
		//stop rendering
		d3ddev->EndScene();
	}
	// display the back buffer on the screen
		d3ddev->Present(NULL, NULL, NULL, NULL);
	//check for escape key (to exit program)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}