#pragma once
#include "physx/include/PxPhysicsAPI.h"

class PhysicsScene
{
	friend class VehiclePhysics;
public:
	PhysicsScene();

private:
	physx::PxDefaultAllocator		gAllocator;
	physx::PxDefaultErrorCallback	gErrorCallback;
	physx::PxFoundation*			gFoundation = NULL;
	physx::PxPhysics*				gPhysics = NULL;
	physx::PxDefaultCpuDispatcher*	gDispatcher = NULL;
	physx::PxScene*				gScene = NULL;
	physx::PxCooking*				gCooking = NULL;
	physx::PxMaterial*				gMaterial = NULL;
	physx::PxPvd*                  gPvd = NULL;
};