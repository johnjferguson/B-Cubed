#pragma once
#include "Renderable.h"
#include <vector>
#include <memory>

class Overlay
{
public:
	Overlay(Graphics& gfx, float screenWidth, float screenHeight);
	void Draw(Graphics& gfx, unsigned short numCharges, unsigned short nLap);
private:
	std::vector<std::unique_ptr<Renderable>> chargeBar;
	std::vector<std::unique_ptr<Renderable>> lapNumber;
	std::unique_ptr<Renderable> map;
private:
	float screenWidth;
	float screenHeight;
	std::vector<float> b;
};
