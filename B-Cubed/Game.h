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

class Game
{
public:
	Game();
	int Start();
	void DoFrame();
	void DoInput();
	void fireMissile(physx::PxVec3 startPos, physx::PxQuat startRot, physx::PxVec3 startVel);
private:
	// order here matter Gui has to be before Gui has to be before Window since there is
	// some dependancy
	Gui gui;
	Window wnd;
	Sound sound;
	//physics;
	Physics ps;

	std::vector<Entity> entities;
	FrameTimer ft;
	std::unique_ptr<FollowCamera> cam0;
private:
	std::vector<std::unique_ptr<Renderable>> skyboxes;
	std::vector<std::unique_ptr<Camera>> cameras;
	RenderTexture renderTexture;
	Light light;
private:
	unsigned int iSkybox = 0u;
	unsigned int activeCamera = 0u;
};
