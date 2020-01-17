#include <Windows.h>
#include "Game.h"

// entry point for windows
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow)
{
	return Game{}.Start();	
}


