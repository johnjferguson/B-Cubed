#pragma once
#include "Renderable.h"
#include <vector>
#include "Controller.h"
#include <memory>

class Menu
{
public:
	Menu() = default;
	Menu(Graphics& gfx, float screenWidth, float screenHeight);
	void Draw(Graphics& gfx, Controller& ctlr, std::vector<int> finish_order);

	bool StartGame = false;

	bool player_1 = false;
	bool player_2 = false;
	bool player_3 = false;
	bool player_4 = false;
private:
	std::vector<std::unique_ptr<Renderable>> chargeBar;
	std::vector<std::unique_ptr<Renderable>> lapNumber;
	std::vector<std::unique_ptr<Renderable>> nPlace;
	std::unique_ptr<Renderable> map;
	std::unique_ptr<Renderable> vehicle;
	std::unique_ptr<Renderable> missile;


	std::unique_ptr<Renderable> p0;
	std::unique_ptr<Renderable> p1;
	std::unique_ptr<Renderable> p2;
	std::unique_ptr<Renderable> p3;
	std::unique_ptr<Renderable> p4;
	std::unique_ptr<Renderable> p12;
	std::unique_ptr<Renderable> p13;
	std::unique_ptr<Renderable> p14;
	std::unique_ptr<Renderable> p23;
	std::unique_ptr<Renderable> p24;
	std::unique_ptr<Renderable> p34;
	std::unique_ptr<Renderable> p123;
	std::unique_ptr<Renderable> p124;
	std::unique_ptr<Renderable> p134;
	std::unique_ptr<Renderable> p234;
	std::unique_ptr<Renderable> p1234;
	std::unique_ptr<Renderable> start;
private:
	float screenWidth;
	float screenHeight;
	std::vector<float> b;

	bool first_load = true;

	bool a_1_pressed_start = false;
	bool a_2_pressed_start = false;
	bool a_3_pressed_start = false;
	bool a_4_pressed_start = false;
};
