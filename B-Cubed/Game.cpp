#include "Game.h"
#include <sstream>
#include "Box.h"
#include "SkyBox.h"
#include <DirectXMath.h>
#include "VehiclePhysics.h"
#include "MissilePhysics.h"

Game::Game()
	:
	wnd(1280, 720, "B-Cubed"),
	light(wnd.gfx, { 10.0f, 10.0f, 10.0f, 1.0f }),
	renderTexture(wnd.gfx.GetDevice(), wnd.GetWidth(), wnd.GetHeight(), 1.0f, 400.0f)
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

	// create physics component
	std::unique_ptr<VehiclePhysics> vp0 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this);
	std::unique_ptr<VehiclePhysics> vp1 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this);

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
	entities[4].AddPhysics(std::move(vp1));
			   
	entities[5].AddRenderable(std::move(nb));
	entities[5].SetPosition(5.0f, 1.0f, 0.0f);
	entities[5].AddPhysics(std::move(vp0));
			   
	entities[6].AddRenderable(std::move(zb));
	entities[6].SetPosition(0.0f, 1.0f, -5.0f);
			   
	entities[7].AddRenderable(std::move(eb));
	entities[7].SetPosition(-5.0f, 1.0f, 0.0f);

	// skyboxes
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox0.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox1.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox2.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox3.png"));

	std::unique_ptr<FollowCamera> cam0 = std::make_unique<FollowCamera>();
	cam0->SetTarget(entities[5]);
	std::unique_ptr<FreeCamera> cam1 = std::make_unique<FreeCamera>(wnd.kbd, wnd.mouse, DirectX::XMFLOAT3( 0.0f,10.0f,10.0f ));
	cameras.push_back(std::move(cam0));
	cameras.push_back(std::move(cam1));

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
	DirectX::XMMATRIX cameraTransform = cameras[activeCamera]->GetTransform(dt);
	
	renderTexture.SetRenderTarget(wnd.gfx.GetContext());
	renderTexture.ClearRenderTarget(wnd.gfx.GetContext());


	for (auto& e : entities)
	{
		e.RenderDepth(wnd.gfx, light.LookAt({ 0.0f,0.0f,0.0f }), renderTexture.GetPerspective(), light);
	}
	

	wnd.gfx.ResetRenderTargetView();
	wnd.gfx.ResetViewPort();
	

	wnd.gfx.GetContext()->PSSetShaderResources(1, 1, renderTexture.GetShaderResourceView());

	for (auto& e : entities)
	{
		e.Render(wnd.gfx, cameraTransform, light.LookAt({ 0.0f,0.0f,0.0f }), renderTexture.GetPerspective(), light);
	}


	//physx::PxVec3 pos = physics.GetPosition();
	//entities[5].SetTransform(physics.GetTransform());
	//entities[5].SetPosition(pos.x, pos.y, pos.z);
	
	struct Transform
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX perspective;
		DirectX::XMMATRIX buf0;
		DirectX::XMMATRIX buf1;
		DirectX::XMFLOAT4 buf3;
	};

	Transform transform
	{
		DirectX::XMMatrixIdentity(),
		cameraTransform,
		DirectX::XMMatrixPerspectiveLH(1.0f, float(wnd.gfx.GetHeight()) / float(wnd.gfx.GetWidth()), 0.5f, 400.0f)
	};

	skyboxes[iSkybox]->UpdateVertex(wnd.gfx, transform);
	skyboxes[iSkybox]->Render(wnd.gfx);

	entities[5].UpdatePhysics();
	entities[4].UpdatePhysics();
	
	for (int i = 8; i < entities.size(); i++) {
		entities[i].UpdatePhysics();
	}
	
	ps.Update(dt);
	//physics.Update(dt(), wnd.clr);
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
				activeCamera = ++activeCamera % cameras.size();
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


void Game::fireMissile(physx::PxVec3 startPos) 
{
	entities.push_back(Entity());


	//std::unique_ptr<VehiclePhysics> vp0 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this);
	std::unique_ptr<MissilePhysics> vp2 = std::make_unique<MissilePhysics>(&ps, startPos);
	std::unique_ptr<Box> vb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//voli.jpg");

	Game::entities[entities.size()-1].AddRenderable(std::move(vb));
	Game::entities[entities.size()-1].SetPosition(startPos.x, startPos.y, startPos.z);
	Game::entities[entities.size()-1].AddPhysics(std::move(vp2));

}
