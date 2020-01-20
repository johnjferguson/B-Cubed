#pragma once
#include <memory>
#include "Renderable.h"

class Entity
{
public:
	Entity() = default;
	void AddRenderable(std::unique_ptr<Renderable> pRenderable);
	void Render(Graphics& gfx);
private:
	std::unique_ptr<Renderable> renderable;
private:
	// TEMP just making sure refactor is working
	float pitch = 0.0f; // rotation around y-axis
	float yaw = 0.0f;   // rotation around z-axis
	float roll = 0.0f;  // rotation around x-axis
	float distance = 10.0f;
};
