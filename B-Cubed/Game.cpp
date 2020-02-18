#include "Game.h"
#include <sstream>
#include "Box.h"
#include "SkyBox.h"
#include <DirectXMath.h>
#include "VehiclePhysics.h"
#include "MissilePhysics.h"
#include "PhysicsStatic.h"

using namespace physx;

Game::Game()
	:
	wnd(1280, 720, "B-Cubed"), 
	light(wnd.gfx, { 10.0f, 40.0f, 10.0f, 1.0f }),
	renderTexture(wnd.gfx.GetDevice(), wnd.GetWidth(), wnd.GetHeight(), 1.0f, 500.0f)
{
	entities = std::vector<Entity>(10);

	//entity.AddRenderable(std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f,2.0f,2.0f), L"images//voli.jpg"));
	std::unique_ptr<Box> bl = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(100.0f, 100.0f, 1.0f), L"images//checker.jpg");
	std::unique_ptr<Box> br = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//neonwall.jpg");
	std::unique_ptr<Box> tl = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//rock.jpg");
	std::unique_ptr<Box> tr = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//wood.jpg");

	std::unique_ptr<Box> w0 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(100.0f, 2.0f, 5.0f), L"images//neonwall.jpg");
	std::unique_ptr<Box> w1 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, -100.0f, 5.0f), L"images//neonwall.jpg");
	std::unique_ptr<Box> w2 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(-100.0f, 2.0f, 5.0f), L"images//neonwall.jpg");
	std::unique_ptr<Box> w3 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 100.0f, 5.0f), L"images//neonwall.jpg");

	std::unique_ptr<Box> vb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//voli.jpg");
	std::unique_ptr<Box> nb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(3.5f, 2.5f, 2.0f), L"images//holovehicle.jpg");
	std::unique_ptr<Box> zb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(3.5f, 2.5f, 2.0f), L"images//holovehicle.jpg");
	std::unique_ptr<Box> zb1 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(3.5f, 2.5f, 2.0f), L"images//holovehicle.jpg");
	std::unique_ptr<Box> zb2 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(3.5f, 2.5f, 2.0f), L"images//holovehicle.jpg");
	std::unique_ptr<Box> eb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//error.png");

	// create physics component

	// Vehicle Physics
	std::unique_ptr<VehiclePhysics> vp0 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, false, 10.0f, 0.0f);
	std::unique_ptr<VehiclePhysics> vp1 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, true, 30.f, 0.f);
	std::unique_ptr<VehiclePhysics> vp2 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, true, 25.f, 10.f);
	std::unique_ptr<VehiclePhysics> vp3 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, true, 10.f, 20.f);

	// Static Physics
	std::unique_ptr<PhysicsStatic> sp0 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), physx::PxVec3(100.0f, 1.0f, 100.0f));
	std::unique_ptr<PhysicsStatic> sp1 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(3.0f, 2.0f, 6.0f)), physx::PxVec3(5.0f,0.5f,5.0f));

	// Static Physics Cont. (Walls)
	std::unique_ptr<PhysicsStatic> sp2 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(-50.0f, 2.5f, 0.0f)), physx::PxVec3(0.5f, 10.0f, 50.0f));
	std::unique_ptr<PhysicsStatic> sp3 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(0.0f, 2.5f, 50.0f)), physx::PxVec3(50.0f, 10.0f, 0.5f));
	std::unique_ptr<PhysicsStatic> sp4 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(50.0f, 2.5f, 0.0f)), physx::PxVec3(0.5f, 10.0f, 50.0f));
	std::unique_ptr<PhysicsStatic> sp5 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(0.0f, 2.5f, -50.0f)), physx::PxVec3(50.0f, 10.0f, 0.5f));

	entities[0].AddRenderable(std::move(bl));
	entities[0].SetPosition(30.0f, 0.0f, 0.0f);
	entities[0].AddPhysics(std::move(sp0));

	entities[1].AddRenderable(std::move(zb));
	entities[1].SetPosition(0.0f, 1.0f, 10.0f);
	entities[1].AddPhysics(std::move(vp1));
			   
	entities[2].AddRenderable(std::move(w0));
	entities[2].SetPosition(-5.0f, -0.5f, 5.0f);
	entities[2].AddPhysics(std::move(sp2));
			   
	entities[3].AddRenderable(std::move(w1));
	entities[3].SetPosition(5.0f, -0.5f, 5.0f);
	entities[3].AddPhysics(std::move(sp3));

	entities[4].AddRenderable(std::move(nb));
	entities[4].SetPosition(10.0f, 1.0f, 0.0f);
	entities[4].AddPhysics(std::move(vp0));
			   
	entities[5].AddRenderable(std::move(br));
	entities[5].SetPosition(0.0f, 1.0f, -5.0f);
	entities[5].AddPhysics(std::move(sp1));
			   
	entities[6].AddRenderable(std::move(w2));
	entities[6].SetPosition(-5.0f, 1.0f, 0.0f);
	entities[6].AddPhysics(std::move(sp4));

	entities[7].AddRenderable(std::move(w3));
	entities[7].SetPosition(-5.0f, 1.0f, 0.0f);
	entities[7].AddPhysics(std::move(sp5));

	entities[8].AddRenderable(std::move(zb1));
	entities[8].SetPosition(-5.0f, 1.0f, 0.0f);
	entities[8].AddPhysics(std::move(vp2));

	entities[9].AddRenderable(std::move(zb2));
	entities[9].SetPosition(-5.0f, 1.0f, 0.0f);
	entities[9].AddPhysics(std::move(vp3));

	// skyboxes
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 200.0f, L"images//skybox0.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 200.0f, L"images//skybox1.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 200.0f, L"images//skybox2.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 200.0f, L"images//skybox3.png"));

	cam0 = std::make_unique<FollowCamera>();
	cam0->SetTarget(entities[4]);
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
	
	Time dt = ft.Set();

	// update physics
	for (int i = 0; i < entities.size(); i++) {
		entities[i].UpdatePhysics();
	}

	// simulate has to be done after physics is updated since vehicle has its own update function
	ps.Simulate(dt);

	// input
	DoInput();

	DirectX::XMMATRIX cameraTransform = cameras[activeCamera]->GetTransform(dt);
	
	// shadow rendering
	renderTexture.SetRenderTarget(wnd.gfx.GetContext());
	renderTexture.ClearRenderTarget(wnd.gfx.GetContext());

	for (auto& e : entities)
	{
		e.RenderDepth(wnd.gfx, light.LookAt({ 0.0f,0.0f,0.0f }), renderTexture.GetPerspective(), light);
	}
	

	wnd.gfx.ResetRenderTargetView();
	wnd.gfx.ResetViewPort();
	
	// bind the depth texture to the pixel shader
	wnd.gfx.GetContext()->PSSetShaderResources(1, 1, renderTexture.GetShaderResourceView());

	for (auto& e : entities)
	{
		e.Render(wnd.gfx, cameraTransform, light.LookAt({ 0.0f,0.0f,0.0f }), renderTexture.GetPerspective(), light);
	}

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
	
	
	light.Update(wnd.gfx, cameraTransform);
	light.Render(wnd.gfx);

	// fetch the physics results for the next frame
	ps.Fetch();

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


void Game::fireMissile(physx::PxVec3 startPos, physx::PxQuat startRot, physx::PxVec3 startVel)
{
	entities.push_back(Entity());

	//std::unique_ptr<MissilePhysics> vp2 = std::make_unique<MissilePhysics>(&ps, startPos, startRot, startVel);
	std::unique_ptr<MissilePhysics> vp2 = std::make_unique<MissilePhysics>(&ps, PxTransform(startPos), startVel, PxVec3(1.0f, 1.0f, 1.0f));
	std::unique_ptr<Box> vb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), L"images//voli.jpg");

	Game::entities[entities.size()-1].AddRenderable(std::move(vb));
	Game::entities[entities.size()-1].SetPosition(startPos.x, startPos.y, startPos.z);
	Game::entities[entities.size()-1].AddPhysics(std::move(vp2)); 
}
