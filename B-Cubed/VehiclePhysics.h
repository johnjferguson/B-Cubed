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
#include "PhysicsScene.h"
#include "Controller.h"

class VehiclePhysics : public PhysicsComponent
{
public:
	VehiclePhysics(PhysicsScene& ps);
	virtual void Update(DirectX::XMFLOAT3& pos, DirectX::XMMATRIX& transform) override;
	void initVehicle(PhysicsScene & ps);
	snippetvehicle::VehicleDesc initVehicleDesc(PhysicsScene & ps);
	void releaseAllControls();
	void stepPhysics(Controller& gameController);
private:
	physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
	physx::PxRigidStatic*			gGroundPlane = NULL;
	physx::PxVehicleDrive4W*		gVehicle4W = NULL;
	bool					gIsVehicleInAir = true;
	std::vector<physx::PxF32> gSteerVsForwardSpeedData;
	physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;
	physx::PxVehicleKeySmoothingData gKeySmoothingData;
	physx::PxVehiclePadSmoothingData gPadSmoothingData;
	physx::PxVehicleDrive4WRawInputData gVehicleInputData;
	physx::PxBatchQuery*			gBatchQuery = NULL;
	snippetvehicle::VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	physx::PxF32					gVehicleModeLifetime = 4.0f;
	physx::PxF32					gVehicleModeTimer = 0.0f;
	physx::PxU32					gVehicleOrderProgress = 0;
	bool					gVehicleOrderComplete = false;
	bool					gMimicKeyInputs = false;
};