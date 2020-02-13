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

class Game
{
public:
	Game();
	int Start();
	void DoFrame();
	void DoInput();
private:
	// keep this order
	Physics physics;
	std::vector<Entity> entities;
private:
	// order here matter Gui has to be before Gui has to be before Window since there is
	// some dependancy
	Gui gui;
	Window wnd;
	//Physics physics;
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
