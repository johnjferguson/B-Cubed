#pragma once
#include "physx/include/PxPhysicsAPI.h"
#include "physx/include/PxShape.h"
#include "Time.h"


class Physics
{
	friend class PhysicsComponent;
	friend class MissilePhysics;
public:
	Physics();
	~Physics();
	void Simulate(const Time& dt);
	void Fetch();
private:
	physx::PxDefaultAllocator		gAllocator;
	physx::PxDefaultErrorCallback	gErrorCallback;
	physx::PxFoundation*			gFoundation = NULL;
	physx::PxPhysics*				gPhysics = NULL;
	physx::PxDefaultCpuDispatcher*	gDispatcher = NULL;
	physx::PxScene*					gScene = NULL;
	physx::PxCooking*				gCooking = NULL;
	physx::PxMaterial*				gMaterial = NULL;
	physx::PxPvd*					gPvd = NULL;
private:
	static constexpr float timestep = 1.0f / 60.0f;
	float current = 0.0f;
};