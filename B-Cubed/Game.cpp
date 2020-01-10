#include "Game.h"

Game::Game()
	:
	wnd(1280, 720, "B-Cubed")
{}

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
	if (wnd.mouse.LeftIsPressed())
		wnd.gfx.TestDraw(wnd.mouse.PosX(), wnd.mouse.PosY());
	else
		wnd.gfx.TestDraw(wnd.GetWidth()/2, wnd.GetHeight()/2);
}
