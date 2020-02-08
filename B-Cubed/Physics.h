//#define PX_PHYSX_STATIC_LIB
#pragma once
#include "physx/include/foundation/PxFoundationConfig.h"
#include <memory>
#include "physx/include/PxPhysicsAPI.h"
#include "Time.h"

#include "physx/include/vehicle/PxVehicleUtil.h"
#include "physx/include/snippetvehiclecommon/SnippetVehicleSceneQuery.h"
#include "physx/include/snippetvehiclecommon/SnippetVehicleFilterShader.h"
#include "physx/include/snippetvehiclecommon/SnippetVehicleTireFriction.h"
#include "physx/include/snippetvehiclecommon/SnippetVehicleCreate.h"

#include "physx/include/snippetcommon/SnippetPrint.h"
#include "physx/include/snippetcommon/SnippetPVD.h"
#include "physx/include//snippetutils/SnippetUtils.h"

class Physics
{
public:
	Physics();
	~Physics();
	void Update(Time dt);
private:
	physx::PxRigidDynamic * CreateDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, const physx::PxVec3& velocity);
	snippetvehicle::VehicleDesc initVehicleDesc();
	void startAccelerateForwardsMode();
	void startAccelerateReverseMode();
	void startBrakeMode();
	void startTurnHardLeftMode();
	void startTurnHardRightMode();
	void startHandbrakeTurnLeftMode();
	void startHandbrakeTurnRightMode();
	void releaseAllControls();
	void initPhysics();
	void incrementDrivingMode(const physx::PxF32 timestep);
	void stepPhysics();
	void cleanupPhysics();
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
	physx::PxCooking* gCooking = NULL;
	physx::PxPvd* gPvd = NULL;

	snippetvehicle::VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	physx::PxBatchQuery*			gBatchQuery = NULL;

	snippetvehicle::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

	physx::PxRigidStatic*			gGroundPlane = NULL;
	physx::PxVehicleDrive4W*		gVehicle4W = NULL;
	physx::PxF32 gSteerVsForwardSpeedData[2 * 8];
	bool					gIsVehicleInAir = true;
	physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;
};
