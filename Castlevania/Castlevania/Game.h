#pragma once
#include "GlobalDefine.h"
#include "Window.h"
#include "Sound.h"
#include "Input.h"
#include "DirectX.h"

class Game :
	protected Window, Sound, Input, DirectX
	
{
private:

public:
	void GameInit();
	void GameRun();
	void GameEnd();

	Game(HINSTANCE hInstance);
	Game();
	~Game();
};

