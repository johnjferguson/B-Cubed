#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Renderable.h"
#include "Light.h"

class Entity
{
public:
	Entity();
	void AddRenderable(std::unique_ptr<Renderable> pRenderable);
	void Render(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightView, const DirectX::XMMATRIX& lightProjection, const Light& light);
	void RenderDepth(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightProjection, const Light& light);

	void SetPosition(float x, float y, float z);
	const DirectX::XMFLOAT3& GetPosition() const;
	void SetTransform(const DirectX::XMMATRIX& transform_in);
	const DirectX::XMMATRIX& GetTransform() const;
private:
	std::unique_ptr<Renderable> renderable;
private:
	// position vector and rotation matrix for object
	DirectX::XMFLOAT3 pos;
	DirectX::XMMATRIX transform;
};
