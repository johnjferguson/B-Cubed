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
	void SetType(Entity::Type typeIn);
	Entity::Type GetType();
	void MarkForDeath();
	bool IsMarkedForDeath();

	void IncreaseHit();
	void ResetHit();
	const int NumberofHits() const;

	void SetPosition(float x, float y, float z);
	const DirectX::XMFLOAT3& GetPosition() const;
	void SetTransform(const DirectX::XMMATRIX& transform_in);
	const DirectX::XMMATRIX& GetTransform() const;
	bool CanParry();
	bool CanBlock();
	void ResetBarrier();
	void CountBarrier();
	void SetBounceBack(bool set);
	void SetSpinOut(bool set);
	bool GetBounceBack() const;
	void SetBounceDir(physx::PxVec3 dir);
	physx::PxVec3 GetBounceDir() const;
	bool GetSpinOut() const;
	int getNumCharges();
	void setNumCharges(int x);
public:
	int lastHitCounter = 0;
private:
	std::unique_ptr<PhysicsComponent> physicsComponent = nullptr;
	std::unique_ptr<Renderable> renderable = nullptr;
private:
	// position vector and rotation matrix for object
	DirectX::XMFLOAT3 pos;
	DirectX::XMMATRIX transform;
	Entity::Type type = Entity::Type::DEFAULT;
	bool markedForDeath = false;
	int hit = 0;
	int	barrierTime = 0;
	bool spinOut = false;
	bool bounceBack = false;
	int numCharges = 0;
	physx::PxVec3 bounceDir;
};