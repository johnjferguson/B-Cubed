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
#include "Controller.h"
#include <vector>
#include "Entity.h"
#include "Game.h"
#include "AI.h"

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
	
	VehiclePhysics(Physics* px, Controller& gameController, Game* game, float startPosX, float startPosY, float startPosZ, int carNum);
	VehiclePhysics(Physics* px, Controller& gameController, Game* game, std::vector<physx::PxVec3> p, float startPosX, float startPosY, float startPosZ, int carNum);
	virtual void Update(Entity* entity, const Time& dt) override;
	virtual void SetPosition(const DirectX::XMFLOAT3& position) override;
	virtual void SetVelocity(const DirectX::XMFLOAT3& velocity) override;
	void initVehicle(Physics* ps);
	snippetvehicle::VehicleDesc initVehicleDesc(Physics* ps);
	void releaseAllControls();
	void stepPhysics(Entity* entity);
	const physx::PxVec3 CurrentPosition();
private:
	float currentTime = 0.0f;
	constexpr static float timestep = 1.0f / 120.0f;
private:
	void applyBoost();
	void spinOut();
	void checkLaps(Entity* entity);
private:
	float							startPosX;
	float							startPosY;
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

	float							abilityCharges = 0;

	float							startRechargeTimer = 0;
	float							rechargeTime = 0;

	float							startAbilityTimer = 0; 
	float							abilityTime = 0;

	float							startSpinOutTimer = 1;
	float							spinOutTime = 99;

	float							startBoostTimer = 1;
	float							boostTime = 99;

	int								carNum;
		
	bool							checkPoint1 = false;
	bool							checkPoint2 = false;
	bool                            checkPoint3 = false;

	Controller& gameController;
	Physics& px;
	std::vector<physx::PxF32>		 gSteerVsForwardSpeedData;
	physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;
	physx::PxVehicleKeySmoothingData gKeySmoothingData;
	physx::PxVehiclePadSmoothingData gPadSmoothingData;

	physx::PxVehicleDrive4WRawInputData gVehicleInputData;
	std::vector<DriveMode>			    gDriveModeOrder;

	Game* game;
	AI ai;
	bool useAI = false;

	bool boost;
	bool barrier;
	bool blast;

	const int setSpinOutTime = 1;   //how long a spinout lasts
	const int setAbilityTime = 2;   //how often player can use an ability
	const int setRechargeTime = 6.7; //how often player gets an ability charge
	const int setBoostTime = 2;     //how long car will boost for
};