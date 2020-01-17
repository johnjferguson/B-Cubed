#pragma once
#include "Window.h"
#include "Gui.h"
#include "Physics.h"

class Game
{
public:
	Game();
	int Start();
	void DoFrame();
private:
	// order here matter Gui has to be before Gui has to be before Window since there is
	// some dependancy
	Gui gui;
	Window wnd;
	Physics physics;
};
