#pragma once
#include "PhysicsComponent.h"
#include "Entity.h"

class PhysicsStatic : public PhysicsComponent
{
public:
	PhysicsStatic(Physics* px, const physx::PxTransform& transform, const physx::PxVec3& dimensions);
	PhysicsStatic() = default;
	virtual ~PhysicsStatic();
	virtual void Update(Entity* entity) override;
protected:
	physx::PxRigidStatic* gRigidStatic;
};