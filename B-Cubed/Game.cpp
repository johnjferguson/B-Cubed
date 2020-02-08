#include "Game.h"
#include <sstream>
#include "Box.h"
#include "SkyBox.h"
#include <DirectXMath.h>

Game::Game()
	:
	wnd(1280, 720, "B-Cubed"),
	camera(wnd.kbd,wnd.mouse,{0.0f,10.0f,10.0f}),
	light(wnd.gfx, { 10.0f, 10.0f, 10.0f, 1.0f }),
	renderTexture(wnd.gfx.GetDevice(), wnd.GetWidth(), wnd.GetHeight(), 1.0f, 200.0f)
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

	// skyboxes
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox0.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox1.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox2.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox3.png"));
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

	Gui::AddText("Press ESC to TOGGLE Free Look Mode");
	Gui::AddText("Press TAB to ROTATE Skyboxes");

	// testing --------------------------------
	DirectX::XMMATRIX cameraTransform = camera.GetTransform(dt);
	
	renderTexture.SetRenderTarget(wnd.gfx.GetContext());
	renderTexture.ClearRenderTarget(wnd.gfx.GetContext());


	for (auto& e : entities)
	{
		e.RenderDepth(wnd.gfx, cameraTransform, renderTexture.GetPerspective(), light);
	}
	

	wnd.gfx.ResetRenderTargetView();
	wnd.gfx.ResetViewPort();
	

	wnd.gfx.GetContext()->PSSetShaderResources(1, 1, renderTexture.GetShaderResourceView());

	for (auto& e : entities)
	{
		e.Render(wnd.gfx, cameraTransform, light.LookAt({ 0.0f,0.0f,0.0f }), renderTexture.GetPerspective(), light);
	}
	
	// testing ---------------------------
	struct Transform
	{
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX perspective;
	};

	Transform transform
	{
		cameraTransform,
		DirectX::XMMatrixPerspectiveLH(1.0f, float(wnd.gfx.GetHeight()) / float(wnd.gfx.GetWidth()), 0.5f, 400.0f)
	};

	skyboxes[iSkybox]->UpdateVertex(wnd.gfx, transform);
	skyboxes[iSkybox]->Render(wnd.gfx);

	physics.Update(dt());
	light.Update(wnd.gfx, cameraTransform);
	light.Render(wnd.gfx);

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
		case VK_TAB:
			if (type == Keyboard::Event::Type::Press)
			{
				iSkybox = ++iSkybox % skyboxes.size();
			}
			break;
		}
	}
}
