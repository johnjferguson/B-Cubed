#include "PhysicsComponent.h"

physx::PxPhysics * PhysicsComponent::GetPhysics(Physics * physics)
{
	return physics->gPhysics;
}

physx::PxScene * PhysicsComponent::GetScene(Physics * physics)
{
	return physics->gScene;
}

physx::PxMaterial * PhysicsComponent::GetMaterial(Physics * physics)
{
	return physics->gMaterial;
}

physx::PxDefaultAllocator * PhysicsComponent::GetAllocator(Physics* physics)
{
	return &physics->gAllocator;
}

physx::PxCooking * PhysicsComponent::GetCooking(Physics* physics)
{
	return physics->gCooking;
}
