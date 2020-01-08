#include "Game.h"

Game::Game()
	:
	wnd(1280,720,"B-Cubed")
{
}

int Game::Start()
{
	while (true)
	{
		if (const auto eCode = wnd.ProcessMessages())
		{
			return *eCode;
		}
	}
}
