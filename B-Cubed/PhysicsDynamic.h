#pragma once
#include "PhysicsComponent.h"
#include "Entity.h"

class PhysicsDynamic : public PhysicsComponent
{
public:
	PhysicsDynamic(Physics* px, const physx::PxTransform& transform, const physx::PxVec3& velocity, const physx::PxVec3& dimensions);
	virtual ~PhysicsDynamic();
	virtual void Update(Entity* entity) override;
protected:
	physx::PxRigidDynamic* gRigidDynamic;
};
