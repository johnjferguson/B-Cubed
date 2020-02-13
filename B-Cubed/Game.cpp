#include "Game.h"
#include <sstream>
#include "Box.h"
#include "SkyBox.h"
#include <DirectXMath.h>
#include "PhysicsDynamic.h"
#include "PhysicsStatic.h"

Game::Game()
	:
	wnd(1280, 720, "B-Cubed"),
	light(wnd.gfx, { 10.0f, 50.0f, 10.0f, 1.0f }),
	renderTexture(wnd.gfx.GetDevice(), wnd.GetWidth(), wnd.GetHeight(), 1.0f, 400.0f)
{

	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//checker.jpg"),
		std::make_unique<PhysicsStatic>(&physics, physx::PxVec3(-5.0f, -0.5f, -5.0f), physx::PxVec3(5.0f, 0.5f, 5.0f))
	);
	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//metal.jpg"),
		std::make_unique<PhysicsStatic>(&physics, physx::PxVec3(5.0f, -0.5f, -5.0f), physx::PxVec3(5.0f, 0.5f, 5.0f))
	);
	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//rock.jpg"),
		std::make_unique<PhysicsStatic>(&physics, physx::PxVec3(-5.0f, -0.5f, 5.0f), physx::PxVec3(5.0f, 0.5f, 5.0f))
	);
	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//wood.jpg"),
		std::make_unique<PhysicsStatic>(&physics, physx::PxVec3(5.0f, -0.5f, 5.0f), physx::PxVec3(5.0f, 0.5f, 5.0f))
	);
	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//voli.jpg"),
		std::make_unique<PhysicsDynamic>(&physics, physx::PxVec3(0.0f, 5.0f, 5.0f), physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(1.0f,1.0f,1.0f))
	);
	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//naut.jpg"),
		std::make_unique<PhysicsDynamic>(&physics, physx::PxVec3(5.0f, 5.0f, 0.0f), physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(1.0f, 1.0f, 1.0f))
	);
	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//zoe.jpg"),
		std::make_unique<PhysicsDynamic>(&physics, physx::PxVec3(0.0f, 5.0f, -5.0f), physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(1.0f, 1.0f, 1.0f))
	);
	entities.emplace_back(
		std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//error.png"),
		std::make_unique<PhysicsDynamic>(&physics, physx::PxVec3(-5.0f, 5.0f, 0.0f), physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(1.0f, 1.0f, 1.0f))
	);

	// skyboxes
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox0.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox1.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox2.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 100.0f, L"images//skybox3.png"));

	cam0 = std::make_unique<FollowCamera>();
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

	physics.Simulate(dt);

	for (auto& e : entities)
	{
		e.UpdatePhysics();
	}

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
	
	//physics.Update(dt(), wnd.clr);
	light.Update(wnd.gfx, cameraTransform);
	light.Render(wnd.gfx);

	gui.End();
	physics.Fetch();
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

