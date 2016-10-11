#include "Game.h"



void Game::GameInit()
{
	this->InitWindow();
	this->InitKeyBoard();
	this->InitDirectX();
}

void Game::GameRun()
{
	MSG msg;

	int done = 0;
	DWORD frame_start = GetTickCount();
	DWORD tick_per_frame = 100 / FrameRate;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//look for quit message
			if (msg.message == WM_QUIT)
				done = 1;
			//decode and pass messages on to WndProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//process game loop (else prevents running after window is closed)
			DWORD now = GetTickCount();
			DeltaTime = now - frame_start;
			if (DeltaTime >= tick_per_frame)
			{
				frame_start = now;
				
			}

			this->ProcessKeyBoard();
			this->ProcessInput(DeltaTime);
		}

	}
	//return msg.wParam;
}

void Game::GameEnd()
{

}


Game::Game(HINSTANCE hInstance) : Window(hInstance)
{

}

Game::Game()
{

}


Game::~Game()
{
}
