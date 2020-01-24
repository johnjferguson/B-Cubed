#include "Game.h"
#include <sstream>
#include "Box.h"
#include <DirectXMath.h>

Game::Game()
	:
	wnd(1280, 720, "B-Cubed"),
	camera(wnd.kbd,wnd.mouse,{0.0f,0.0f,0.0f})
{
	entity.AddRenderable(std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f,2.0f,2.0f), L"images//voli.jpg"));
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
	wnd.gfx.StartFrame();
	gui.Begin("B-Cubed gui window");

	Time dt = ft.Set();

	std::stringstream ss;
	float mdt = dt.Milliseconds();
	ss << "my frame timer: " << mdt;
	Gui::AddText(ss.str());

	// mouse pos does nothing right now
	entity.Render(wnd.gfx, camera.GetTransform(dt));
	//wnd.gfx.TestDraw(wnd.mouse.PosX(), wnd.mouse.PosY());
	physics.Update(dt());

	gui.End();
	wnd.gfx.EndFrame();
}
