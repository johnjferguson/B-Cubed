#pragma once
#include "PhysicsComponent.h"
#include "Entity.h"

class PhysicsStatic : public PhysicsComponent
{
public:
	PhysicsStatic(Physics* px, const physx::PxTransform& transform, const physx::PxVec3& dimensions);
	virtual ~PhysicsStatic();
	virtual void Update(Entity* entity) override;
protected:
	physx::PxRigidStatic* gRigidStatic;
};