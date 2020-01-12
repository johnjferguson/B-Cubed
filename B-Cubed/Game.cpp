#include "Game.h"

Game::Game()
	:
	wnd(1280, 720, "B-Cubed")
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
		DoFrame();
	}
}

void Game::DoFrame()
{
	gui.Begin("B-Cubed gui window");

	// mouse pos does nothing right now
	wnd.gfx.TestDraw(wnd.mouse.PosX(), wnd.mouse.PosY());

	gui.End();
	wnd.gfx.EndFrame();
}
