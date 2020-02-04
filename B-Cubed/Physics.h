#pragma once
#include <memory>
#include "physx/include/PxPhysicsAPI.h"
#include "Time.h"

class Physics
{
public:
	Physics();
	~Physics();
	void Update(Time dt);
private:
	physx::PxRigidDynamic * CreateDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, const physx::PxVec3& velocity);
private:
	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;
	physx::PxFoundation* gFoundation = NULL;
	physx::PxPhysics* gPhysics = NULL;
	physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
	physx::PxScene*	gScene = NULL;
	physx::PxMaterial* gMaterial = NULL;
	physx::PxCudaContextManager* gCudaContextManager = NULL;
	physx::PxRigidDynamic* ball = NULL;
	physx::PxPvd* gPvd = NULL;
};
