#include "Input.h"

int Input::InitKeyBoard()
{
	
	HResult = DirectInput8Create(HInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&Di, NULL);
	if (HResult != D3D_OK)
		return 0;
	HResult = Di->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);
	if (HResult != D3D_OK)
		return 0;
	HResult = Keyboard->SetDataFormat(&c_dfDIKeyboard);
	HResult = Keyboard->SetCooperativeLevel(HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size


	HResult = Keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (HResult != DI_OK) return 0;

	HResult = Keyboard->Acquire();
	if (HResult != DI_OK) return 0;
	return 1;
}

int Input::IsKeyDown(int KeyCode)
{
	return (KeyStates[KeyCode] & 0x80) > 0;
}

void Input::OnKeyDown(int KeyCode)
{
	
	/*switch (KeyCode)
	{
	case DIK_SPACE:
		break;
	}*/
}

void Input::OneKeyPress(int Delta)
{
	/*if (IsKeyDown(DIK_LEFT))
	{
		boxRECT->Vx = (-GRAVITY);
	}
	*/
}

void Input::OnKeyUp(int KeyCode)
{

}

void Input::ProcessInput(int Delta)
{
	OneKeyPress(Delta);
}

void Input::ProcessKeyBoard()
{
	Keyboard->GetDeviceState(sizeof(KeyStates), KeyStates);
	// trang thay cua phim tai thoi diem chung ta goi ham
	// nếu như bắn đạn thì phải dùng công cụ khác sử dung buffer sác định từng phím


	if (IsKeyDown(DIK_ESCAPE))
	{
		PostMessage(HWnd, WM_QUIT, 0, 0);
	}

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	HResult = Keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), KeyEvents, &dwElements, 0);

	// Scan through all data, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = KeyEvents[i].dwOfs;
		int KeyState = KeyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			OnKeyDown(KeyCode);
		else
			OnKeyUp(KeyCode);
	}
}

Input::Input()
{
}

Input::~Input()
{
}
