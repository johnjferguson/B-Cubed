#pragma once
#include "Window.h"

class Game
{
public:
	Game();
	int Start();
	void DoFrame();
private:
	Window wnd;
};
