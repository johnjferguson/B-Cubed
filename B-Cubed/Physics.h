#pragma once
#include <memory>
#include "physx/include/PxPhysicsAPI.h"
#include "Time.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleCreate.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleSceneQuery.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleTireFriction.h"

class Physics
{
public:
	Physics();
	~Physics();
	void Update(Time dt);
private:
	physx::PxRigidDynamic * CreateDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, const physx::PxVec3& velocity);
	snippetvehicle::VehicleDesc initVehicleDesc();
	void startBrakeMode();
	void initPhysics();
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
	physx::PxF32					gVehicleModeLifetime = 4.0f;
	physx::PxF32					gVehicleModeTimer = 0.0f;
	physx::PxU32					gVehicleOrderProgress = 0;
	bool					gVehicleOrderComplete = false;
	bool					gMimicKeyInputs = false;
	physx::PxRigidStatic*			gGroundPlane = NULL;
	physx::PxVehicleDrive4W*		gVehicle4W = NULL;
	physx::PxCooking*				gCooking = NULL;
	physx::PxPvd*                  gPvd = NULL;
	snippetvehicle::VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	physx::PxBatchQuery*			gBatchQuery = NULL;
	physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
	physx::PxVehicleDrive4WRawInputData gVehicleInputData;
};
