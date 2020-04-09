#pragma once
#include <vector>
#include <memory>
#include "Window.h"
#include "Gui.h"
#include "FrameTimer.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "RenderTexture.h"
#include "Physics.h"
#include "Mesh.h"
#include "Sound.h"
#include "Overlay.h"
#include "Menu.h"
#include "EntityManager.h"
#include "Box.h"
#include "PhysicsStatic.h"
#include "PhysicsGround.h"

class Game
{
public:
	Game();
	int Start();
	void DoFrame();
	void InitializeGame();
	void Reset();
	void DoInput();
	void fireMissile(physx::PxVec3 startPos, physx::PxQuat startRot, physx::PxVec3 startVel);
public:
	float gameCounter = 0.f;
private:
	// order here matter Gui has to be before Gui has to be before Window since there is
	// some dependancy
	Gui gui;
	Window wnd;
	Sound sound;
	//physics;
	Physics ps;
	//std::vector<Entity> entities;
	FrameTimer ft;
	std::unique_ptr<FollowCamera> cam0;
	bool first;
	bool second;
	bool setup_done = false;
	bool take_down_done = false;
private:
	std::vector<std::unique_ptr<Renderable>> skyboxes;
	std::vector<std::unique_ptr<Camera>> cameras;
	RenderTexture renderTexture;
	Light light;
	Overlay overlay;
	Menu menu;
	EntityManager entityManager = EntityManager();
	std::vector<D3D11_VIEWPORT> viewports;
	std::vector<std::vector<unsigned int>> viewportsPerPlayers;
	std::vector<unsigned int> vehicleIds;
private:
	unsigned int iSkybox = 0u;
	unsigned int activeCamera = 0u;
	DirectX::XMFLOAT3 startPosition;
	unsigned int nPlayers = 1;
	bool AI_1 = true;
	bool AI_2 = true;
	bool AI_3 = true;
	bool AI_4 = true;

	float start_end = 0; 

	std::vector<int> finish_order;
private:
	std::unique_ptr<Mesh> bl;
	std::unique_ptr<Mesh> b2;
	std::unique_ptr<Mesh> b3;
	std::unique_ptr<Mesh> b4;
	std::unique_ptr<Mesh> b5;
	std::unique_ptr<Mesh> b6;
	std::unique_ptr<Mesh> b7;
	std::unique_ptr<Mesh> b8;
	std::unique_ptr<Mesh> b9;

	std::unique_ptr<Box> start;
	std::unique_ptr<Box> tl;
	std::unique_ptr<Box> tr;

	std::unique_ptr<Box> w0;
	std::unique_ptr<Box> w1;
	std::unique_ptr<Box> w2;
	std::unique_ptr<Box> w3;

	std::unique_ptr<Box> vb;
	std::unique_ptr<Mesh> nb;
	std::unique_ptr<Mesh> zb;
	std::unique_ptr<Mesh> zb1;
	std::unique_ptr<Mesh> zb2;
	std::unique_ptr<Box> eb;

	std::vector<physx::PxVec3> aipath;

	std::unique_ptr<PhysicsStatic> sp1;
	std::unique_ptr<PhysicsStatic> sp2;
	std::unique_ptr<PhysicsStatic> sp3;
	std::unique_ptr<PhysicsStatic> sp4;
	std::unique_ptr<PhysicsStatic> sp5;

	std::unique_ptr<PhysicsGround> sp0;
	std::unique_ptr<PhysicsGround> sp02;
	std::unique_ptr<PhysicsGround> sp03;
	std::unique_ptr<PhysicsGround> sp04;
	std::unique_ptr<PhysicsGround> sp05;
	std::unique_ptr<PhysicsGround> sp06;
	std::unique_ptr<PhysicsGround> sp07;
	std::unique_ptr<PhysicsGround> sp08;
	std::unique_ptr<PhysicsGround> sp09;
};
