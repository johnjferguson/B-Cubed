#pragma once
#include "PhysicsComponent.h"
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
#include <DirectXMath.h>
#include "Physics.h"

using namespace physx;
using namespace snippetvehicle;

class PhysicsVehicle : public PhysicsComponent
{
public:
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
public:
	PhysicsVehicle(Physics* physics);
	virtual void Update(Entity* entity) override;
	Physics* physics = nullptr;
private:
	VehicleDesc initVehicleDesc();
	void startAccelerateForwardsMode();
	void startAccelerateReverseMode();
	void startBrakeMode();
	void startTurnHardLeftMode();
	void startTurnHardRightMode();
	void startHandbrakeTurnLeftMode();
	void startHandbrakeTurnRightMode();
	void releaseAllControls();
	void initPhysics();
	void incrementDrivingMode(const PxF32 timestep);
	void stepPhysics();
private:
	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	//PxFoundation*			gFoundation = NULL;
	//PxPhysics*				gPhysics = NULL;

	PxDefaultCpuDispatcher*	gDispatcher = NULL;
	//PxScene*				gScene = NULL;

	PxCooking*				gCooking = NULL;

	PxMaterial*				gMaterial = NULL;

	PxPvd*                  gPvd = NULL;

	VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	PxBatchQuery*			gBatchQuery = NULL;

	PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

	PxRigidStatic*			gGroundPlane = NULL;
	PxVehicleDrive4W*		gVehicle4W = NULL;

	bool					gIsVehicleInAir = true;

	std::vector<PxF32> gSteerVsForwardSpeedData;
	PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;
	PxVehicleKeySmoothingData gKeySmoothingData;
	PxVehiclePadSmoothingData gPadSmoothingData;
	PxVehicleDrive4WRawInputData gVehicleInputData;
	std::vector<DriveMode> gDriveModeOrder;

	PxF32					gVehicleModeLifetime = 4.0f;
	PxF32					gVehicleModeTimer = 0.0f;
	PxU32					gVehicleOrderProgress = 0;
	bool					gVehicleOrderComplete = false;
	bool					gMimicKeyInputs = false;
};