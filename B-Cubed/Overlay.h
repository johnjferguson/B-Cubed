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
	void Draw(Graphics& gfx, unsigned short numCharges, unsigned short nLap, int place, EntityManager& entityManager, const std::vector<unsigned int>& entities);
private:
	std::vector<std::unique_ptr<Renderable>> chargeBar;
	std::vector<std::unique_ptr<Renderable>> lapNumber;
	std::vector<std::unique_ptr<Renderable>> nPlace;
	std::unique_ptr<Renderable> map;
	std::unique_ptr<Renderable> vehicle;
	std::unique_ptr<Renderable> missile;
private:
	float screenWidth;
	float screenHeight;
	std::vector<float> b;
};
