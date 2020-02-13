#pragma once
#include "PhysicsComponent.h"
#include "Physics.h"


class PhysicsStatic : public PhysicsComponent
{
public:
	PhysicsStatic(Physics* phy, const physx::PxVec3& position, const physx::PxVec3& dimensions);
	~PhysicsStatic();
	virtual void Update(Entity* entity) override;
private:
	physx::PxRigidStatic* gRigidStatic;
};
