#pragma once
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	Game *game = new Game(hInstance);

	game->GameInit();
	game->GameRun();
	game->GameEnd();
}

