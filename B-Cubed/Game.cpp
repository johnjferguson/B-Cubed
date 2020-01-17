#include "Game.h"
#include <sstream>

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
	gui.Begin("B-Cubed gui window");

	Time dt = ft.Set();

	std::stringstream ss;
	float mdt = dt.Milliseconds();
	ss << "my frame timer: " << mdt;
	Gui::AddText(ss.str());

	// mouse pos does nothing right now
	wnd.gfx.TestDraw(wnd.mouse.PosX(), wnd.mouse.PosY());
	physics.Update(dt());

	gui.End();
	wnd.gfx.EndFrame();
}
