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
};
