#include "Game.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Box.h"
#include "SkyBox.h"
#include <DirectXMath.h>
#include "VehiclePhysics.h"
#include "MissilePhysics.h"
#include "PhysicsStatic.h"
#include "PhysicsGround.h"

using namespace physx;
namespace dx = DirectX;

Game::Game()
	:
	wnd(1280, 720, "B-Cubed"), 
	light(wnd.gfx, { 0.0f, 0.0f, 100.0f, 1.0f }),
	renderTexture(wnd.gfx.GetDevice(), wnd.GetWidth(), wnd.GetHeight(), 1.0f, 700.0f)
{
	//entities = std::vector<Entity>(10);
	//entities.reserve(110);

	//std::unique_ptr<Mesh> nb = std::make_unique<Mesh>(wnd.gfx, 1.0f, "models//simpletrack.obj");

	//entity.AddRenderable(std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f,2.0f,2.0f), L"images//voli.jpg"));
	//std::unique_ptr<Box> bl = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(100.0f, 100.0f, 1.0f), L"images//checker.jpg");
	//std::unique_ptr<Mesh> bl = std::make_unique<Mesh>(wnd.gfx, 2.0f, "models//evenbettertrack.obj");
	std::unique_ptr<Mesh> bl = std::make_unique<Mesh>(wnd.gfx, 3.0f, "models//SPLINE2.obj");
	std::unique_ptr<Mesh> b2 = std::make_unique<Mesh>(wnd.gfx, 3.0f, "models//blank1.obj");
	std::unique_ptr<Box> start = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(33.0f, 8.0f, 1.0f), L"images//chess.jpg");
	std::unique_ptr<Box> tl = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//rock.jpg");
	std::unique_ptr<Box> tr = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(10.0f, 10.0f, 1.0f), L"images//wood.jpg");

	std::unique_ptr<Box> w0 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(100.0f, 2.0f, 5.0f), L"images//neonwall.jpg");
	std::unique_ptr<Box> w1 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, -100.0f, 5.0f), L"images//neonwall.jpg");
	std::unique_ptr<Box> w2 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(-100.0f, 2.0f, 5.0f), L"images//neonwall.jpg");
	std::unique_ptr<Box> w3 = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 100.0f, 5.0f), L"images//neonwall.jpg");

	std::unique_ptr<Box> vb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//voli.jpg");
	//std::unique_ptr<Mesh> nb = std::make_unique<Mesh>(wnd.gfx, 1.0f, "models//simpletrack.obj");
	std::unique_ptr<Mesh> nb = std::make_unique<Mesh>(wnd.gfx, 1.0f, "models//vehicle.obj");
	std::unique_ptr<Mesh> zb = std::make_unique<Mesh>(wnd.gfx, 1.0f, "models//vehicle.obj");
	std::unique_ptr<Mesh> zb1 = std::make_unique<Mesh>(wnd.gfx, 1.0f, "models//vehicle.obj");
	std::unique_ptr<Mesh> zb2 = std::make_unique<Mesh>(wnd.gfx, 1.0f, "models//vehicle.obj");
	std::unique_ptr<Box> eb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), L"images//error.png");

	// create physics component
	//startPosition = dx::XMFLOAT3(-200.f, 15.0f, 141.0f);
	startPosition = dx::XMFLOAT3(-175.f, 15.0f, 202.0f);
	// Vehicle Physics
	//std::unique_ptr<VehiclePhysics> player_vehicle = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, -201.f, 15.0f, 138.0f, 0);

	//std::unique_ptr<VehiclePhysics> player_vehicle = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, -47.f, 11.0f, -112.0f, 0);
	std::unique_ptr<VehiclePhysics> player_vehicle = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, -55.f, 11.0f, -108.0f, 0);

	std::vector<physx::PxVec3> aipath;
	aipath.push_back({ 158, -10, 144 });
	aipath.push_back({ 212, -10, 119 });
	aipath.push_back({ 251, -10, 44 });
	aipath.push_back({ 242, -10, -69 });
	aipath.push_back({ 198, -10, -115 });
	aipath.push_back({ 128, -10, -128 });
	aipath.push_back({ - 149, -10, -129 });
	aipath.push_back({ - 200, -10, -117 });
	aipath.push_back({ - 240, -10, -78 });
	aipath.push_back({ - 261, -10, -7 });
	aipath.push_back({ - 237, -10, 104 });
	aipath.push_back({ - 174, -10, 139 });
	/*
	std::unique_ptr<VehiclePhysics> ai_vehicle_1 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -203.f, 15.0f, 149.f, 1);
	//std::unique_ptr<VehiclePhysics> ai_vehicle_2 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -179.3f, 13.2f, 193.3f, 2);
	std::unique_ptr<VehiclePhysics> ai_vehicle_2 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -166.3f, 16.f, 174.f, 2);
	//std::unique_ptr<VehiclePhysics> ai_vehicle_2 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -187.f, 15.f, 171.f, 2);
	std::unique_ptr<VehiclePhysics> ai_vehicle_3 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -187.3f, 15.0f, 153.f, 3);
	*/

	std::unique_ptr<VehiclePhysics> ai_vehicle_1 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -105.f, 9.f, -125.f, 1);
	//std::unique_ptr<VehiclePhysics> ai_vehicle_2 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -97.f, 10.3f, -112.f, 2);
	std::unique_ptr<VehiclePhysics> ai_vehicle_2 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -80.f, 10.3f, -119.f, 2);
	std::unique_ptr<VehiclePhysics> ai_vehicle_3 = std::make_unique<VehiclePhysics>(&ps, wnd.clr, this, aipath, -66.f, 10.3f, -113.f, 3);

	// Static Physics
	//std::unique_ptr<PhysicsStatic> sp0 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), physx::PxVec3(100.0f, 1.0f, 100.0f));
	std::unique_ptr<PhysicsStatic> sp1 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(3.0f, 2.0f, 6.0f)), physx::PxVec3(5.0f,0.5f,5.0f));

	// Static Physics Cont. (Walls)
	std::unique_ptr<PhysicsStatic> sp2 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(-50.0f, 2.5f, 0.0f)), physx::PxVec3(0.5f, 10.0f, 50.0f));
	std::unique_ptr<PhysicsStatic> sp3 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(0.0f, 2.5f, 50.0f)), physx::PxVec3(50.0f, 10.0f, 0.5f));
	std::unique_ptr<PhysicsStatic> sp4 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(50.0f, 2.5f, 0.0f)), physx::PxVec3(0.5f, 10.0f, 50.0f));
	std::unique_ptr<PhysicsStatic> sp5 = std::make_unique<PhysicsStatic>(&ps, PxTransform(physx::PxVec3(0.0f, 2.5f, -50.0f)), physx::PxVec3(50.0f, 10.0f, 0.5f));

	auto vertices = bl->GetVertices();
	auto indices = bl->GetIndices();
	std::unique_ptr<PhysicsGround> sp0 = std::make_unique<PhysicsGround>(&ps, PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), vertices, indices, true);

	auto vertices2 = b2->GetVertices();
	auto indices2 = b2->GetIndices();
	std::unique_ptr<PhysicsGround> sp02 = std::make_unique<PhysicsGround>(&ps, PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)), vertices2, indices2, false);

	entityManager.Add(std::make_unique<Entity>(std::move(bl), std::move(sp0), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::DEFAULT));		
	entityManager.Add(std::make_unique<Entity>(std::move(b2), std::move(sp02), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::DEFAULT));							// 0
	//entityManager.Add(std::make_unique<Entity>(std::move(zb), std::move(ai_vehicle_1), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::VEHICLE));							// 1
	entityManager.Add(std::make_unique<Entity>(std::move(w0), std::move(sp2), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::DEFAULT));							// 2
	entityManager.Add(std::make_unique<Entity>(std::move(w1), std::move(sp3), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::DEFAULT));							// 3
	playerId = entityManager.Add(std::make_unique<Entity>(std::move(nb), std::move(player_vehicle), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::VEHICLE));				// 4
	entityManager.Add(std::make_unique<Entity>(std::move(start), std::move(sp1), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::DEFAULT));							// 5
	entityManager.Add(std::make_unique<Entity>(std::move(w2), std::move(sp4), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::DEFAULT));							// 6
	entityManager.Add(std::make_unique<Entity>(std::move(w3), std::move(sp5), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::VEHICLE));							// 7
	//entityManager.Add(std::make_unique<Entity>(std::move(zb1), std::move(ai_vehicle_2), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::VEHICLE));							// 8
	//entityManager.Add(std::make_unique<Entity>(std::move(zb2), std::move(ai_vehicle_3), dx::XMFLOAT3{ 0.0f,0.0f,0.0f }, dx::XMMatrixIdentity(), Entity::Type::VEHICLE));						    // 9

	// skyboxes
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 700.0f, L"images//skybox0.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 700.0f, L"images//skybox1.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 700.0f, L"images//skybox2.png"));
	skyboxes.push_back(std::make_unique<SkyBox>(wnd.gfx, 700.0f, L"images//skybox3.png"));

	cam0 = std::make_unique<FollowCamera>(entityManager, playerId);
	std::unique_ptr<FreeCamera> cam1 = std::make_unique<FreeCamera>(wnd.kbd, wnd.mouse, DirectX::XMFLOAT3( 0.0f,10.0f,10.0f ));
	cameras.push_back(std::move(cam0));
	cameras.push_back(std::move(cam1));

	D3D11_VIEWPORT vp;
	vp.Width = float(wnd.GetWidth());
	vp.Height = float(wnd.GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	viewports.push_back(vp);

	vp.Height = float(wnd.GetHeight()) / 2.0f;
	viewports.push_back(vp);

	vp.TopLeftY = float(wnd.GetHeight()) / 2.0f;
	viewports.push_back(vp);

	vp.TopLeftY = 0.0f;
	vp.Width = float(wnd.GetWidth()) / 2.0f;
	viewports.push_back(vp);

	vp.TopLeftX = float(wnd.GetWidth()) / 2.0f;
	viewports.push_back(vp);

	vp.TopLeftX = 0.0f;
	vp.TopLeftY = float(wnd.GetHeight()) / 2.0f;
	viewports.push_back(vp);

	vp.TopLeftX = float(wnd.GetWidth()) / 2.0f;
	viewports.push_back(vp);

	viewportsPerPlayers.push_back({ 0 });
	viewportsPerPlayers.push_back({ 1,2 });
	viewportsPerPlayers.push_back({ 1,5,6 });
	viewportsPerPlayers.push_back({ 3,4,5,6 });

	overlay = Overlay(wnd.gfx, wnd.GetWidth(), wnd.GetHeight());
}

int Game::Start()
{
	
	Sound::Load("sounds//bulletbounce.wav", 0.5, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
	Sound::Load("sounds//bounce2.wav", 0.5, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
	Sound::Load("sounds//barrier.wav", 0.2f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
	Sound::Load("sounds//blast.wav", 0.2f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
	Sound::Load("sounds//boost.wav", 0.1f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
	
	first = true;
	second = true;


	//entityManager.Get(playerId)->SetPosition(startPosition.x, startPosition.y, startPosition.z);
	//entityManager.Get(playerId)->SetVelocity({ 0.0f, 0.0f, 0.0f });

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
	//entityManager.UpdatePhysics(dt);

	// simulate has to be done after physics is updated since vehicle has its own update function
	ps.Simulate(dt, entityManager);

	// input
	DoInput();

	DirectX::XMMATRIX cameraTransform = cameras[activeCamera]->GetTransform(dt);


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
		DirectX::XMMatrixPerspectiveLH(1.0f, float(wnd.gfx.GetHeight()) / float(wnd.gfx.GetWidth()), 0.5f, 700.0f)
	};

	for (auto& i : viewportsPerPlayers[nPlayers - 1])
	{

		// shadow rendering
		renderTexture.SetRenderTarget(wnd.gfx.GetContext());
		renderTexture.ClearRenderTarget(wnd.gfx.GetContext());

		dx::XMFLOAT3 pos = entityManager.Get(playerId)->GetPosition();

		std::stringstream ss;
		ss << "Position: " << (int)pos.x << " :  " << (int)pos.y << " : " << (int)pos.z;
		Gui::AddText(ss.str().c_str());

		static float x = 1.0f;
		static float y = 1.0f;
		static float z = 1.0f;
		Gui::AddSlider("x:", x, -100, 100);
		Gui::AddSlider("y:", y, -100, 100);
		Gui::AddSlider("z:", z, -100, 100);

		light.SetPosition(DirectX::XMFLOAT4(pos.x + x, pos.y + y, pos.z, z));

		entityManager.RenderDepth(wnd.gfx, light.LookAt({ pos.x, pos.y, pos.z }), renderTexture.GetPerspective(), light);

		wnd.gfx.ResetRenderTargetView();
		wnd.gfx.ResetViewPort();

		// bind the depth texture to the pixel shader
		wnd.gfx.GetContext()->PSSetShaderResources(1, 1, renderTexture.GetShaderResourceView());

		wnd.gfx.SetViewPort(viewports[i]);
		entityManager.Render(wnd.gfx, cameraTransform, light.LookAt({ pos.x, pos.y, pos.z }), renderTexture.GetPerspective(), light);

		skyboxes[iSkybox]->UpdateVertex(wnd.gfx, transform);
		skyboxes[iSkybox]->Render(wnd.gfx);

		//light.Update(wnd.gfx, cameraTransform);
		//light.Render(wnd.gfx);

		std::vector<unsigned int> vehicles = entityManager.Query(Entity::Type::VEHICLE);
		std::vector<unsigned int>  missiles = entityManager.Query(Entity::Type::MISSILE);
		vehicles.insert(vehicles.end(), std::make_move_iterator(missiles.begin()), std::make_move_iterator(missiles.end()));

		overlay.Draw(wnd.gfx, entityManager.Get(playerId)->getNumCharges(), 
			entityManager.Get(playerId)->GetNumLaps(), 
			entityManager.Get(playerId)->getFinishedIn(),
		    entityManager, vehicles);
		wnd.gfx.ResetViewPort();
	}
	
	//light.Update(wnd.gfx, cameraTransform);
	//light.Render(wnd.gfx);

	// fetch the physics results for the next frame
	//ps.Fetch();

	entityManager.Cull();

	gameCounter += dt.Seconds();

	std::stringstream ss;
	ss << "Time: " << gameCounter;
	Gui::AddText(ss.str().c_str());

	gameCounter += dt.Seconds();


	if (first) {
		first = false;
		Sound::Play("sounds//countdown.wav", 0.5f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
	}
	else if (gameCounter > 16.5 && second) {
		second = false;
		Sound::Play("sounds//BackgroundLoop.wav", 0.1f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), true);
	}

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
		case 'R':
			entityManager.Get(playerId)->SetPosition(startPosition.x,startPosition.y,startPosition.z);
			entityManager.Get(playerId)->SetVelocity({ 0.0f, 0.0f, 0.0f });
			break;
		case '1':
			nPlayers = 1;
			break;
		case '2':
			nPlayers = 2;
			break;
		case '3':
			nPlayers = 3;
			break;
		case '4':
			nPlayers = 4;
			break;
		}
	}
}


void Game::fireMissile(physx::PxVec3 startPos, physx::PxQuat startRot, physx::PxVec3 startVel)
{
	//entities.push_back(Entity());

	//std::unique_ptr<MissilePhysics> ai_vehicle_2 = std::make_unique<MissilePhysics>(&ps, startPos, startRot, startVel);

	DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(startRot.x, startRot.y, startRot.z, startRot.w)).r[2];
	PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), 0, DirectX::XMVectorGetZ(mat));

	PxTransform missileTrans;

	if (startVel.magnitude() <= 40) {
		missileTrans = PxTransform(PxVec3(startPos.x, startPos.y + 1.f, startPos.z) + forward * 8.0f);
	}
	else if (startVel.magnitude() <= 80) {
		missileTrans = PxTransform(PxVec3(startPos.x, startPos.y + 3.f, startPos.z) + forward * 12.0f);
	}
	else {
		missileTrans = PxTransform(PxVec3(startPos.x, startPos.y + 5.f, startPos.z) + forward * 15.0f);
	}
	//PxTransform missileTrans = PxTransform(startPos + PxVec3(0.0f, 5.0f, 0.0f));
	PxVec3 missileVel = (forward * 120.f + startVel);

	std::unique_ptr<MissilePhysics> vp2 = std::make_unique<MissilePhysics>(&ps, missileTrans, missileVel, PxVec3(1.0f, 1.0f, 1.0f));
	std::unique_ptr<Box> vb = std::make_unique<Box>(wnd.gfx, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), L"images//voli.jpg");

	//Game::entities[entities.size()-1].AddRenderable(std::move(vb));
	//Game::entities[entities.size()-1].SetPosition(startPos.x + forward.x, startPos.y, startPos.z + forward.z);
	//Game::entities[entities.size()-1].AddPhysics(std::move(vp2)); 
	//Game::entities[entities.size()-1].SetType(Entity::Type::MISSILE); 
	entityManager.Add(std::make_unique<Entity>(std::move(vb), std::move(vp2), dx::XMFLOAT3{ startPos.x + forward.x, startPos.y, startPos.z + forward.z }, dx::XMMatrixIdentity(), Entity::Type::MISSILE));
}
