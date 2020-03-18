#pragma once
#include <DirectXMath.h>
#include "Physics.h"

class PhysicsComponent
{
public:
	virtual ~PhysicsComponent() = default;
	virtual void Update(class Entity* entity ) = 0;
	virtual void SetPosition(const DirectX::XMFLOAT3& position) = 0;
	virtual void SetVelocity(const DirectX::XMFLOAT3& velocity) = 0;
protected:
	physx::PxPhysics* GetPhysics(Physics* physics);
	physx::PxScene*   GetScene(Physics* physics);
	physx::PxMaterial* GetMaterial(Physics* physics);
	physx::PxDefaultAllocator* GetAllocator(Physics* physics);
	physx::PxCooking* GetCooking(Physics* physics);
private:
};