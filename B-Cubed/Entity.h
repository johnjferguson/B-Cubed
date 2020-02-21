#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Renderable.h"
#include "Light.h"
#include "PhysicsComponent.h"

class Entity
{
public:
	enum class Type
	{
		MISSILE,
		VEHICLE,
		DEFAULT
	};
public:
	Entity();
	void AddRenderable(std::unique_ptr<Renderable> pRenderable);
	void AddPhysics(std::unique_ptr<PhysicsComponent> pPhysics);
	void Render(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightView, const DirectX::XMMATRIX& lightProjection, const Light& light);
	void RenderDepth(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightProjection, const Light& light);
	void UpdatePhysics();

	void SetPosition(float x, float y, float z);
	const DirectX::XMFLOAT3& GetPosition() const;
	void SetTransform(const DirectX::XMMATRIX& transform_in);
	const DirectX::XMMATRIX& GetTransform() const;
private:
	std::unique_ptr<PhysicsComponent> physicsComponent = nullptr;
	std::unique_ptr<Renderable> renderable = nullptr;
private:
	// position vector and rotation matrix for object
	DirectX::XMFLOAT3 pos;
	DirectX::XMMATRIX transform;
	Entity::Type type = Entity::Type::DEFAULT;
};
