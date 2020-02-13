#pragma once
#include "physx/include/PxPhysicsAPI.h"
#include "Time.h"

class Physics
{
	friend class PhysicsStatic;
	friend class PhysicsDynamic;

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
	physx::PxScene*				    gScene = NULL;
	physx::PxPvd*                   gPvd = NULL;
	physx::PxMaterial*				gMaterial = NULL;
private:
	float current = 0.0f;
	static constexpr float timestep = 1.0f / 60.0f;
};
