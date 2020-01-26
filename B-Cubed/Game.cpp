#include "Game.h"
#include <sstream>
#include "Box.h"
#include <DirectXMath.h>

Game::Game()
	:
	wnd(1280, 720, "B-Cubed"),
	camera(wnd.kbd,wnd.mouse,{0.0f,10.0f,10.0f})
{
	entities = std::vector<Entity>(8);

	//entity.AddRenderable(std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f,2.0f,2.0f), L"images//voli.jpg"));
	std::unique_ptr<Box> bl = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//checker.jpg");
	std::unique_ptr<Box> br = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//metal.jpg");
	std::unique_ptr<Box> tl = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//rock.jpg");
	std::unique_ptr<Box> tr = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//wood.jpg");

	std::unique_ptr<Box> vb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//voli.jpg");
	std::unique_ptr<Box> nb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//naut.jpg");
	std::unique_ptr<Box> zb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//zoe.jpg");
	std::unique_ptr<Box> eb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//error.png");

	entities[0].AddRenderable(std::move(bl));
	entities[0].SetPosition(-5.0f, -0.5f, -5.0f);

	entities[1].AddRenderable(std::move(br));
	entities[1].SetPosition(5.0f, -0.5f, -5.0f);
			   
	entities[2].AddRenderable(std::move(tl));
	entities[2].SetPosition(-5.0f, -0.5f, 5.0f);
			   
	entities[3].AddRenderable(std::move(tr));
	entities[3].SetPosition(5.0f, -0.5f, 5.0f);
			   
	entities[4].AddRenderable(std::move(vb));
	entities[4].SetPosition(0.0f, 1.0f, 5.0f);
			   
	entities[5].AddRenderable(std::move(nb));
	entities[5].SetPosition(5.0f, 1.0f, 0.0f);
			   
	entities[6].AddRenderable(std::move(zb));
	entities[6].SetPosition(0.0f, 1.0f, -5.0f);
			   
	entities[7].AddRenderable(std::move(eb));
	entities[7].SetPosition(-5.0f, 1.0f, 0.0f);

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

	DoInput();

	Time dt = ft.Set();

	std::stringstream ss;
	float mdt = dt.Milliseconds();
	ss << "my frame timer: " << mdt;
	Gui::AddText(ss.str());

	
	DirectX::XMMATRIX cameraTransform = camera.GetTransform(dt);
	for (auto& e : entities)
	{
		e.Render(wnd.gfx, cameraTransform);
	}


	physics.Update(dt());

	gui.End();
	wnd.gfx.EndFrame();
}

void Game::DoInput()
{
	while (!wnd.kbd.IsEmpty())
	{
		Keyboard::Event e = wnd.kbd.PopEvent();
		Keyboard::Event::Type type = e.GetType();
		unsigned char c = e.GetCharacter();

		switch (c)
		{
		case VK_ESCAPE:
			if (type == Keyboard::Event::Type::Press)
			{
				camera.ToggleInput();
			}
			break;
		}
	}
}
