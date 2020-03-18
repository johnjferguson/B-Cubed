#pragma once
#include "PhysicsComponent.h"
#include "Entity.h"

class PhysicsDynamic : public PhysicsComponent
{
public:
	PhysicsDynamic(Physics* px, const physx::PxTransform& transform, const physx::PxVec3& velocity, const physx::PxVec3& dimensions);
	virtual void SetPosition(const DirectX::XMFLOAT3& position) override;
	virtual void SetVelocity(const DirectX::XMFLOAT3& velocity) override;
	virtual ~PhysicsDynamic();
	virtual void Update(Entity* entity) override;
protected:
	physx::PxRigidDynamic* gRigidDynamic;
};
