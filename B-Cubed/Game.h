#pragma once
#include <vector>
#include <memory>
#include "Window.h"
#include "Gui.h"
#include "Physics.h"
#include "FrameTimer.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "RenderTexture.h"

class Game
{
public:
	Game();
	int Start();
	void DoFrame();
	void DoInput();
private:
	// order here matter Gui has to be before Gui has to be before Window since there is
	// some dependancy
	Gui gui;
	Window wnd;
	Physics physics;
	FrameTimer ft;
private:
	std::vector<Entity> entities;
	std::vector<std::unique_ptr<Renderable>> skyboxes;
	RenderTexture renderTexture;
	FreeCamera camera;
	Light light;
private:
	unsigned int iSkybox = 0u;
};
