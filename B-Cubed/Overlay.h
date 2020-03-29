#pragma once
#include "Renderable.h"
#include <vector>
#include <memory>
#include "EntityManager.h"

class Overlay
{
public:
	Overlay() = default;
	Overlay(Graphics& gfx, float screenWidth, float screenHeight, const std::vector<unsigned int>& vehicles, EntityManager& entityManager);
	void Draw(Graphics& gfx, unsigned short numCharges, unsigned short nLap, int place, EntityManager& entityManager);
private:
	std::vector<std::unique_ptr<Renderable>> chargeBar;
	std::vector<std::unique_ptr<Renderable>> lapNumber;
	std::vector<std::unique_ptr<Renderable>> nPlace;
	std::vector<std::unique_ptr<Renderable>> arrows;
	std::unique_ptr<Renderable> map;
private:
	float screenWidth;
	float screenHeight;
	std::vector<float> b;
	const std::vector<unsigned int>* pVId = nullptr;
};
