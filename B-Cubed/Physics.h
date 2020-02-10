#pragma once
#include <memory>
#include "physx/include/PxPhysicsAPI.h"
#include "Time.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleCreate.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleSceneQuery.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleTireFriction.h"
#include "physx/vehicle4W/snippetcommon/SnippetPVD.h"
#include "physx/include/vehicle/PxVehicleUtil.h"
#include "physx/include/snippetutils/SnippetUtils.h"

class Physics
{
public:
	Physics();
	~Physics();
	void Update(Time dt);
	physx::PxVec3 GetPosition();

	enum class DriveMode
	{
		eDRIVE_MODE_ACCEL_FORWARDS = 0,
		eDRIVE_MODE_ACCEL_REVERSE,
		eDRIVE_MODE_HARD_TURN_LEFT,
		eDRIVE_MODE_HANDBRAKE_TURN_LEFT,
		eDRIVE_MODE_HARD_TURN_RIGHT,
		eDRIVE_MODE_HANDBRAKE_TURN_RIGHT,
		eDRIVE_MODE_BRAKE,
		eDRIVE_MODE_NONE
	};
private:
	physx::PxRigidDynamic * CreateDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, const physx::PxVec3& velocity);
	snippetvehicle::VehicleDesc initVehicleDesc();
	void startBrakeMode();
	void initVehicle();
	void keyPress(unsigned char key, const physx::PxTransform& camera);
	void startAccelerateForwardsMode();
	void startAccelerateReverseMode();
	void startTurnHardLeftMode();
	void startTurnHardRightMode();
	void startHandbrakeTurnLeftMode();
	void startHandbrakeTurnRightMode();
	void releaseAllControls();
	void incrementDrivingMode(const physx::PxF32 timestep);
	void stepPhysics();
	void cleanupPhysics();
private:
	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;

	physx::PxFoundation* gFoundation = NULL;			// Wrapper that goes around allocator and error callback
	physx::PxPhysics* gPhysics = NULL;					// Does the physics simulation
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

	physx::PxPvd*                  gPvd = NULL;			// Visual Debugger
	snippetvehicle::VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	physx::PxBatchQuery*			gBatchQuery = NULL;
	physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
	physx::PxVehicleDrive4WRawInputData gVehicleInputData;
	
	physx::PxF32 gSteerVsForwardSpeedData[2 * 8];
	bool					gIsVehicleInAir = true;
	physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;

	DriveMode gDriveModeOrder[8];

	physx::PxVehicleKeySmoothingData gKeySmoothingData;
	physx::PxVehiclePadSmoothingData gPadSmoothingData;

	int currentMode = 0; 
};
