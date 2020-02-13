#pragma once
#include "PhysicsComponent.h"
#include "Physics.h"

class PhysicsDynamic : public PhysicsComponent
{
public:
	PhysicsDynamic(Physics* phy, const physx::PxVec3& position, const physx::PxVec3& velocity, const physx::PxVec3& dimensions);
	~PhysicsDynamic();
	virtual void Update(Entity* entity) override;
private:
	physx::PxRigidDynamic* gRigidDynamic;
};
