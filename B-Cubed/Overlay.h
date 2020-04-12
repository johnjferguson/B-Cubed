#pragma once
#include "Renderable.h"
#include <vector>
#include <memory>
#include "EntityManager.h"


class Overlay
{
public:
	Overlay() = default;
	Overlay(Graphics& gfx, float screenWidth, float screenHeight);
	void Draw(Graphics& gfx, unsigned short numChargesBoost, unsigned short numChargesBarrier, unsigned short numChargesBlast, unsigned short nLap, int place, EntityManager& entityManager, const std::vector<unsigned int>& entities, int carNum);
private:
	std::vector<std::unique_ptr<Renderable>> chargeBarBoost;
	std::vector<std::unique_ptr<Renderable>> chargeBarBarrier;
	std::vector<std::unique_ptr<Renderable>> chargeBarBlast;
	std::vector<std::unique_ptr<Renderable>> lapNumber;
	std::vector<std::unique_ptr<Renderable>> nPlace;
	std::unique_ptr<Renderable> map;
	std::unique_ptr<Renderable> vehicle;
	std::unique_ptr<Renderable> vehicle_1;
	std::unique_ptr<Renderable> vehicle_2;
	std::unique_ptr<Renderable> vehicle_3;
	std::unique_ptr<Renderable> vehicle_4;
	std::unique_ptr<Renderable> missile;
	std::unique_ptr<Renderable> icon_1;
	std::unique_ptr<Renderable> icon_2;
	std::unique_ptr<Renderable> icon_3;
	std::unique_ptr<Renderable> icon_4;
private:
	float screenWidth;
	float screenHeight;
	std::vector<float> b;
};
