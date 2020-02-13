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
#include <vector>
#include "Entity.h"
#include "Game.h"

class VehiclePhysics : public PhysicsComponent
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
	VehiclePhysics(PhysicsScene* ps, Controller& gameController, Game* game, bool AI, float startPosX, float startPosZ);
	virtual void Update(DirectX::XMFLOAT3& pos, DirectX::XMMATRIX& transform) override;
	void initVehicle(PhysicsScene* ps);
	snippetvehicle::VehicleDesc initVehicleDesc(PhysicsScene* ps);
	void releaseAllControls();
	void stepPhysics();
private:
	void applyBoost();
private:
	float							startPosX;
	float							startPosZ;
	physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
	physx::PxRigidStatic*			gGroundPlane = NULL;
	physx::PxVehicleDrive4W*		gVehicle4W = NULL;
	bool							gIsVehicleInAir = true;
	physx::PxBatchQuery*			gBatchQuery = NULL;
	snippetvehicle::VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	physx::PxF32					gVehicleModeLifetime = 4.0f;
	physx::PxF32					gVehicleModeTimer = 0.0f;
	physx::PxU32					gVehicleOrderProgress = 0;
	bool							gVehicleOrderComplete = false;
	bool							gMimicKeyInputs = false;
	// OnPress switches to true when button is relased
	// This ensures that you press the button to initiate an ability not just hold down the button
	bool							yOnPress = true;
	bool							bOnPress = true;
	bool							aOnPress = true;
	bool							xOnPress = true;

	bool							AI = false;
	int								readyToFire = 0;
	Controller& gameController;
	PhysicsScene& ps;
	std::vector<physx::PxF32>		 gSteerVsForwardSpeedData;
	physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;
	physx::PxVehicleKeySmoothingData gKeySmoothingData;
	physx::PxVehiclePadSmoothingData gPadSmoothingData;

	physx::PxVehicleDrive4WRawInputData gVehicleInputData;
	std::vector<DriveMode>			    gDriveModeOrder;

	Game* game;

	bool reverse;
	bool accel;
	float steer;

	bool boost;
	bool barrier;
	bool blast;

	bool boosting;	// is true when vehicle is currently boosting
	int boostTimer; // counts down while boosting -> boosting stops at 0
};