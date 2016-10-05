#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MessageBox(NULL, "Motoko Kusanagi has hacked your system!",
		"Public Security Section 9", MB_YESNO | MB_ICONEXCLAMATION);
}


