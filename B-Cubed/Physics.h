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
#include <vector>

class Physics
{
public:
	Physics();
	~Physics();
	void Update(Time dt);
	physx::PxVec3 GetPosition();

	enum DriveMode
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
	physx::PxDefaultAllocator		gAllocator;
	physx::PxDefaultErrorCallback	gErrorCallback;

	physx::PxFoundation*			gFoundation = NULL;
	physx::PxPhysics*				gPhysics = NULL;

	physx::PxDefaultCpuDispatcher*	gDispatcher = NULL;
	physx::PxScene*				gScene = NULL;

	physx::PxCooking*				gCooking = NULL;

	physx::PxMaterial*				gMaterial = NULL;

	physx::PxPvd*                  gPvd = NULL;

	snippetvehicle::VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	physx::PxBatchQuery*			gBatchQuery = NULL;

	physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

	physx::PxRigidStatic*			gGroundPlane = NULL;
	physx::PxVehicleDrive4W*		gVehicle4W = NULL;

	bool					gIsVehicleInAir = true;

	std::vector<physx::PxF32> gSteerVsForwardSpeedData;
	physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;
	physx::PxVehicleKeySmoothingData gKeySmoothingData;
	physx::PxVehiclePadSmoothingData gPadSmoothingData;

	physx::PxVehicleDrive4WRawInputData gVehicleInputData;
	std::vector<DriveMode> gDriveModeOrder;

	physx::PxVehicleKeySmoothingData gKeySmoothingData;
	physx::PxVehiclePadSmoothingData gPadSmoothingData;
};
