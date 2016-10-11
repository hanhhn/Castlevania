#pragma once
#include "GlobalDefine.h"
#include <dinput.h>
#include <d3d9.h>


#define KEYBOARD_BUFFER_SIZE 1024
#define KEY_DOWN(code) ( IsKeyDown(code) )

class Input
{
private:
	LPDIRECTINPUT8 Di = NULL;

	LPDIRECTINPUTDEVICE8 Keyboard;
	BYTE  KeyStates[256];
	DIDEVICEOBJECTDATA KeyEvents[KEYBOARD_BUFFER_SIZE];

private:
	int IsKeyDown(int KeyCode);
	void OnKeyDown(int KeyCode);
	void OneKeyPress(int Delta);
	void OnKeyUp(int KeyCode);

protected:
	int InitKeyBoard();
	void ProcessKeyBoard();
	void ProcessInput(int Delta);

public:
	
	Input();
	~Input();
};

