#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Renderable.h"
#include "Light.h"

class Entity
{
public:
	Entity() = default;
	void AddRenderable(std::unique_ptr<Renderable> pRenderable);
	void Render(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightView, const DirectX::XMMATRIX& lightProjection, const Light& light);
	void RenderDepth(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightProjection, const Light& light);

	void SetPosition(float x, float y, float z);
private:
	std::unique_ptr<Renderable> renderable;
private:
	// TEMP just making sure refactor is working
	DirectX::XMFLOAT3 pos;
	float pitch = 0.0f; // rotation around y-axis
	float yaw = 0.0f;   // rotation around z-axis
	float roll = 3.1415f;  // rotation around x-axis
	float distance = 10.0f;
};
