#include "VehiclePhysics.h"
#include "Gui.h"
#include <sstream>
#include "Time.h"

using namespace physx;
using namespace snippetvehicle;

VehiclePhysics::VehiclePhysics(Physics* px, Controller& gameController, Game* game, float startPosX, float startPosY, float startPosZ, int carNum)
	:
	gameController(gameController),
	px(*px)
{
	VehiclePhysics::carNum = carNum;

	VehiclePhysics::game = game;
	VehiclePhysics::startPosX = startPosX;
	VehiclePhysics::startPosY = startPosY;
	VehiclePhysics::startPosZ = startPosZ;

	gSteerVsForwardSpeedData =
	{
		0.0f,		0.70f,
		5.0f,		0.70f,
		30.0f,		0.70f,
		120.0f,		0.70f,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32
	};

	gSteerVsForwardSpeedTable = PxFixedSizeLookupTable<8>(gSteerVsForwardSpeedData.data(), 4);
	gKeySmoothingData =
	{
		{
			3.0f,	//rise rate eANALOG_INPUT_ACCEL
			9.0f,	//rise rate eANALOG_INPUT_BRAKE		
			6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
			2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
			2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
		},
		{
			15.0f,	//fall rate eANALOG_INPUT_ACCEL
			10.0f,	//fall rate eANALOG_INPUT_BRAKE		
			10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
			5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
			5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
		}
	};

	gPadSmoothingData =
	{
		{
			6.0f,	//rise rate eANALOG_INPUT_ACCEL
			6.0f,	//rise rate eANALOG_INPUT_BRAKE		
			6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
			2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
			2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
		},
		{
			10.0f,	//fall rate eANALOG_INPUT_ACCEL
			10.0f,	//fall rate eANALOG_INPUT_BRAKE		
			10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
			5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
			5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
		}
	};

	gDriveModeOrder =
	{
		eDRIVE_MODE_BRAKE,
		eDRIVE_MODE_ACCEL_FORWARDS,
		eDRIVE_MODE_BRAKE,
		eDRIVE_MODE_ACCEL_REVERSE,
		eDRIVE_MODE_BRAKE,
		eDRIVE_MODE_HARD_TURN_LEFT,
		eDRIVE_MODE_BRAKE,
		eDRIVE_MODE_HARD_TURN_RIGHT,
		eDRIVE_MODE_ACCEL_FORWARDS,
		eDRIVE_MODE_HANDBRAKE_TURN_LEFT,
		eDRIVE_MODE_ACCEL_FORWARDS,
		eDRIVE_MODE_HANDBRAKE_TURN_RIGHT,
		eDRIVE_MODE_NONE
	};

	initVehicle(px);
}

VehiclePhysics::VehiclePhysics(Physics* px, Controller& gameController, Game* game, std::vector<physx::PxVec3> p, float startPosX, float startPosY, float startPosZ, int carNum)
	:
	VehiclePhysics(px, gameController, game, startPosX, startPosY, startPosZ, carNum) 
{
	VehiclePhysics::useAI = true;
	VehiclePhysics::ai = AI::AI(p, gVehicle4W);
	setRechargeTime = 3.2f; //how often ai gets an ability charge
}

void VehiclePhysics::Update(Entity* entity, const Time& dt)
{
	entity->SetVelocity(*(DirectX::XMFLOAT3*)&(gVehicle4W->getRigidDynamicActor()->getLinearVelocity()));


	gVehicle4W->getRigidDynamicActor()->userData = (void*)entity;

	PxTransform transform = gVehicle4W->getRigidDynamicActor()->getGlobalPose();

	PxVec3 position = transform.p;
	entity->SetPosition(position.x, position.y, position.z);

	PxQuat quint = transform.q;
	entity->SetTransform(DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)));

	auto j = gVehicle4W->mDriveDynData.getCurrentGear();
	auto p = gVehicle4W->mDriveDynData.getEngineRotationSpeed();
	auto m = gVehicle4W->computeForwardSpeed();

	//std::stringstream ss;
	//ss << int(j) << "   :  " << (int)p << " Forwards Velocity:  " << (int)m;
	//Gui::AddText(ss.str().c_str());

	entity->setNumCharges(abilityCharges);

	PxVec3 ang_vel = gVehicle4W->getRigidDynamicActor()->getAngularVelocity();
	//gVehicle4W->getRigidDynamicActor()->setAngularVelocity(PxVec3(ang_vel.x / 3.0, ang_vel.y / 1.05, ang_vel.z / 3.0));
	gVehicle4W->getRigidDynamicActor()->setAngularVelocity(PxVec3(ang_vel.x / 1.1f, ang_vel.y / 1.1f, ang_vel.z / 1.1f));

	PxVec3 cur_vel = gVehicle4W->getRigidDynamicActor()->getLinearVelocity();
	//gVehicle4W->getRigidDynamicActor()->setLinearVelocity(PxVec3(cur_vel.x, cur_vel.y, cur_vel.z));

	
	//PxVec3 vHorizontal = PxVec3(v.x, 0.0, v.y).getNormalized();

	//PxVec3 p = gRigidDynamic->getGlobalPose().p;
	//PxQuat q = gRigidDynamic->getGlobalPose().q;

	//PxRaycastBuffer hit;
	//bool status = phy->gScene->raycast(PxVec3(p.x, p.y - 3, p.z), PxVec3(0, -1, 0), 50, hit);

	//float dist = hit.block.distance;

	//gRigidDynamic->setGlobalPose(PxTransform(p.x, p.y - (dist - 1), p.z, q));

	PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
	DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[1];
	PxVec3 trackDir = PxVec3(DirectX::XMVectorGetX(mat), DirectX::XMVectorGetY(mat), DirectX::XMVectorGetZ(mat));
	
	if (gIsVehicleInAir) {
		//Gui::AddText("Is in Air");
		//gVehicle4W->getRigidDynamicActor()->setAngularVelocity(PxVec3(ang_vel.x / 3.0, ang_vel.y / 1.05, ang_vel.z / 3.0));
		//gVehicle4W->getRigidDynamicActor()->setLinearVelocity(PxVec3(cur_vel.x, cur_vel.y / 2.0, cur_vel.z));
		//PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
		//DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[1];
		//PxVec3 trackDir = PxVec3(DirectX::XMVectorGetX(mat), DirectX::XMVectorGetY(mat), DirectX::XMVectorGetZ(mat));

		gVehicle4W->getRigidDynamicActor()->addForce(PxVec3(0, 1, 0) * -100000);
		gVehicle4W->getRigidDynamicActor()->addForce(trackDir * -100000);
	}
	else {
		gVehicle4W->getRigidDynamicActor()->addForce(trackDir * -100000);
	}
	

	checkLaps(entity);
	/*
	currentTime += dt();
	while (currentTime > timestep)
	{
		currentTime -= timestep;
	}
	*/
	stepPhysics(entity);

	rechargeTime = game->gameCounter - startRechargeTimer;

	abilityTime = game->gameCounter - startAbilityTimer;

	spinOutTime = game->gameCounter - startSpinOutTimer;

	boostTime = game->gameCounter - startBoostTimer;
}

void VehiclePhysics::SetPosition(const DirectX::XMFLOAT3 & position)
{

	PxTransform current = gVehicle4W->getRigidDynamicActor()->getGlobalPose();
	current.p = PxVec3(position.x, position.y, position.z);
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(current);
}

void VehiclePhysics::SetVelocity(const DirectX::XMFLOAT3 & velocity)
{
	gVehicle4W->getRigidDynamicActor()->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
}

void VehiclePhysics::initVehicle(Physics* px)
{
	//Create the batched scene queries for the suspension raycasts.
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, 1, WheelSceneQueryPreFilterBlocking, NULL, *GetAllocator(px));
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, GetScene(px));

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = createFrictionPairs(GetMaterial(px));

	//Create a plane to drive on.
	//PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	//gGroundPlane = createDrivablePlane(groundPlaneSimFilterData, GetMaterial(px), GetPhysics(px));
	//GetScene(px)->addActor(*gGroundPlane);

	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initVehicleDesc(px);
	gVehicle4W = createVehicle4W(vehicleDesc, GetPhysics(px), GetCooking(px));
	//PxTransform startTransform(PxVec3(startPosX, startPosY, startPosZ), PxQuat(0, 0.3907311, 0, 0.9205049));
	PxTransform startTransform(PxVec3(startPosX, startPosY, startPosZ), PxQuat(0, 0.6225146, 0, -0.7826082));
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	GetScene(px)->addActor(*gVehicle4W->getRigidDynamicActor());

	PxVehicleEngineData eng = gVehicle4W->mDriveSimData.getEngineData();
	eng.mPeakTorque = 3900.f;
	eng.mMaxOmega = 1700;
	//eng.mTorqueCurve = 1;
	//eng.mMOI = 5;

	PxVehicleAckermannGeometryData acker = gVehicle4W->mDriveSimData.getAckermannGeometryData();
	acker.mAccuracy = 1.0;

	// Higer Values = More Torque To Wheels
	PxVehicleClutchData clu = gVehicle4W->mDriveSimData.getClutchData();
	clu.mStrength = gVehicle4W->mDriveSimData.getClutchData().mStrength + 1;

	PxVehicleDifferential4WData diff = gVehicle4W->mDriveSimData.getDiffData();
	diff.mType = PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD;
	//diff.mType = PxVehicleDifferential4WData::eDIFF_TYPE_LS_FRONTWD;
	//diff.mType = PxVehicleDifferential4WData::eDIFF_TYPE_LS_REARWD;

	gVehicle4W->mDriveSimData.setEngineData(eng);
	gVehicle4W->mDriveSimData.setAckermannGeometryData(acker);
	gVehicle4W->mDriveSimData.setClutchData(clu);
	gVehicle4W->mDriveSimData.setDiffData(diff);

	PxVehicleGearsData gearShift = gVehicle4W->mDriveSimData.getGearsData();;
	gearShift.mSwitchTime = 0.0f;
	gVehicle4W->mDriveSimData.setGearsData(gearShift);

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	gVehicle4W->setToRestState();
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eSECOND);
	gVehicle4W->mDriveDynData.setUseAutoGears(true);

	//PxVehicleAutoBoxData mAutoBox;
	//gVehicle4W->mDriveSimData.getAutoBoxData
	/*
	PxTransform com = gVehicle4W->getRigidDynamicActor()->getCMassLocalPose();
	PxTransform offset = PxTransform(PxVec3(0.0, -1.5, 0.0) + com.p);
	gVehicle4W->getRigidDynamicActor()->setCMassLocalPose(offset);
	*/

	gVehicleModeTimer = 0.0f;
	gVehicleOrderProgress = 0;

	gVehicleInputData.setAnalogBrake(true);
	//startBrakeMode();
}

snippetvehicle::VehicleDesc VehiclePhysics::initVehicleDesc(Physics* px)
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	//const PxF32 chassisMass = 1260.0f;
	const PxF32 chassisMass = 2300.0f;
	//const PxVec3 chassisDims(5.5f, 3.0f, 8.0f);
	//const PxVec3 chassisDims(2.75f, 1.5f, 4.0f);
	const PxVec3 chassisDims(5.0f, 4.0f, 7.0f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.7f*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65, 0.2f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 80.0f;
	//const PxF32 wheelRadius = 1.5f;
	//const PxF32 wheelWidth = 1.3f;
	const PxF32 wheelRadius = 0.8f;
	const PxF32 wheelWidth = 0.3f;
	const PxF32 wheelMOI = 0.4f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 4;

	VehicleDesc vehicleDesc;

	vehicleDesc.chassisMass = chassisMass;
	vehicleDesc.chassisDims = chassisDims;
	vehicleDesc.chassisMOI = chassisMOI;
	vehicleDesc.chassisCMOffset = chassisCMOffset;
	vehicleDesc.chassisMaterial = GetMaterial(px);
	vehicleDesc.chassisSimFilterData = PxFilterData(COLLISION_FLAG_CHASSIS, COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);

	vehicleDesc.wheelMass = wheelMass;
	vehicleDesc.wheelRadius = wheelRadius;
	vehicleDesc.wheelWidth = wheelWidth;
	vehicleDesc.wheelMOI = wheelMOI;
	vehicleDesc.numWheels = nbWheels;
	vehicleDesc.wheelMaterial = GetMaterial(px);
	vehicleDesc.chassisSimFilterData = PxFilterData(COLLISION_FLAG_WHEEL, COLLISION_FLAG_WHEEL_AGAINST, 0, 0);

	return vehicleDesc;
}

void VehiclePhysics::releaseAllControls()
{
	if (gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(false);
		gVehicleInputData.setDigitalSteerLeft(false);
		gVehicleInputData.setDigitalSteerRight(false);
		gVehicleInputData.setDigitalBrake(false);
		gVehicleInputData.setDigitalHandbrake(false);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(0.0f);
		gVehicleInputData.setAnalogSteer(0.0f);
		gVehicleInputData.setAnalogBrake(0.0f);
		gVehicleInputData.setAnalogHandbrake(0.0f);
	}
}

void VehiclePhysics::stepPhysics(Entity* entity)
{
	bool accel = false;
	bool reverse = false;
	float steer = 0;

	PxVec3 pos = gVehicle4W->getRigidDynamicActor()->getGlobalPose().p;
	PxQuat qua = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;

	if (!useAI) {
		//::stringstream ss;
		//ss << "Position: " <<  (int)pos.x << " :  " << (int)pos.y << " : " << (int)pos.z;
		//Gui::AddText(ss.str().c_str());

		//std::stringstream oo;
		//oo << "Rotation: " << qua.x << " :  " << qua.y << " : " << qua.z << " : " << (float)qua.w;
		//Gui::AddText(oo.str().c_str());
	}

	if (game->gameCounter > 16) {

		if (useAI) {
			PxVec3 vel = gVehicle4W->getRigidDynamicActor()->getLinearVelocity();
			PxVec3 vehicle_position = gVehicle4W->getRigidDynamicActor()->getGlobalPose().p;
			PxQuat quint = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			DirectX::XMVECTOR dirVec = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)).r[2];

			ai.update();

			accel = ai.getAcceleration();
			reverse = ai.getBrake();
			steer = ai.getSteering();

			blast = ai.getBlast();
			boost = ai.getBoost();
			barrier = ai.getBarrier();
		}
		else {
			PxVec3 vehicle_position = gVehicle4W->getRigidDynamicActor()->getGlobalPose().p;

			accel = gameController.IsPressed(Controller::Button::R_TRIGGER);
			reverse = gameController.IsPressed(Controller::Button::L_TRIGGER);
			steer = pow(gameController.GetLeftStick().x,4);
			if (gameController.GetLeftStick().x < 0)
				steer *= -1;

			blast = gameController.IsPressed(Controller::Button::Y);
			boost = gameController.IsPressed(Controller::Button::B);
			barrier = gameController.IsPressed(Controller::Button::A);
		}
	}
	else {
		gVehicleInputData.setAnalogBrake(true);
	}

	//const PxF32 timestep = 1.0f / 120.0f;

	entity->CountBarrier(game->gameCounter);

	//If full charge or begining of match we don't start counting
	if (abilityCharges >= 3 || game->gameCounter < 16) {
		startRechargeTimer = game->gameCounter;
	}

	//Add an ability charge 
	if (rechargeTime > setRechargeTime && abilityCharges < 3) {
		abilityCharges++;
		startRechargeTimer = game->gameCounter;
	}

	//If hit with a bullet vehicle will spinout
	if (entity->GetSpinOut()) {
		startSpinOutTimer = game->gameCounter;
		entity->SetSpinOut(false);
	}

	if (spinOutTime < setSpinOutTime) {
		spinOut();
	}

	//Cycle through the driving modes to demonstrate how to accelerate/reverse/brake/turn etc.
	//incrementDrivingMode(timestep);
	if (barrier)
	{

		if (abilityTime > setAbilityTime && abilityCharges > 0 && aOnPress) {
			entity->ResetBarrier(game->gameCounter); 
			abilityCharges--;
			if (!useAI) {
				Sound::Play("sounds//barrier.wav", 0.2f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
			}
		}

		aOnPress = false;
	}
	else
	{
		aOnPress = true;
	}

	if (boost)
	{
		if (abilityTime > setAbilityTime && abilityCharges > 0 && bOnPress) {
			if (boostTime > setBoostTime) {
				startBoostTimer = game->gameCounter;
				startAbilityTimer = game->gameCounter;
				abilityCharges--;
			}
		}
		bOnPress = false;
	}
	else
	{
		bOnPress = true;
	}
	if (boostTime < setBoostTime) {
			applyBoost();

	}

	if (blast)
	{
		if (abilityTime > setAbilityTime && abilityCharges > 0 && yOnPress) {
			PxQuat transform = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			//DirectX::XMMATRIX transform = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w));

			if (!useAI) {
				Sound::Play("sounds//blast.wav", 0.2f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);

			}

			game->fireMissile(gVehicle4W->getRigidDynamicActor()->getGlobalPose().p, transform, gVehicle4W->getRigidDynamicActor()->getLinearVelocity());

			startAbilityTimer = game->gameCounter;
			abilityCharges--;
		}
		yOnPress = false;
	}
	else
	{
		yOnPress = true;
	}
	if (gameController.IsPressed(Controller::Button::X))
	{
	}
	else
	{
	}

	if (reverse)
	{
		if ((int)gVehicle4W->computeForwardSpeed() <= 1) {
			gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);
			gVehicleInputData.setAnalogAccel(true);
			gVehicleInputData.setAnalogBrake(false);
		}
		else {
			gVehicleInputData.setAnalogBrake(true);
			PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[2];
			PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), DirectX::XMVectorGetY(mat), DirectX::XMVectorGetZ(mat));

			gVehicle4W->getRigidDynamicActor()->addForce(-55000.f * forward);
		}
	}
	else if (accel)
	{
		if ((int)gVehicle4W->computeForwardSpeed() <= 1) {
			gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
		}
		else if ((int)gVehicle4W->computeForwardSpeed() >= 1 && (int)gVehicle4W->computeForwardSpeed() <= 30) {
			PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[2];
			PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), 0, DirectX::XMVectorGetZ(mat));
			//-----------------------------------------------------------------------------------------------
			gVehicle4W->getRigidDynamicActor()->addForce(10000.f * forward);
			//---------------------------------------------------------------------------------------------------
		}
		else if ((int)gVehicle4W->computeForwardSpeed() >= 1 && (int)gVehicle4W->computeForwardSpeed() <= 60) {
			PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[2];
			PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), 0, DirectX::XMVectorGetZ(mat));
			//-----------------------------------------------------------------------------------------------
			gVehicle4W->getRigidDynamicActor()->addForce(8000.f * forward);
			//---------------------------------------------------------------------------------------------------
		}
		else if ((int)gVehicle4W->computeForwardSpeed() >= 1 && (int)gVehicle4W->computeForwardSpeed() <= 80) {
			PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[2];
			PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), 0, DirectX::XMVectorGetZ(mat));
			//-----------------------------------------------------------------------------------------------
			gVehicle4W->getRigidDynamicActor()->addForce(5000.f * forward);
			//---------------------------------------------------------------------------------------------------
		}

		gVehicleInputData.setAnalogAccel(true);
		gVehicleInputData.setAnalogBrake(false);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(false);
	}

	gVehicleInputData.setAnalogSteer(steer);

	// TEMP: just testing stuff since I don't have xbox controller
	//gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	//gVehicleInputData.setAnalogAccel(true);
	//gVehicleInputData.setAnalogSteer(0.2f);
	// --------------------------------------------------------
	//"Left Trigger (x,y): (" << lt.x << "," << lt.y << ") Right Trigger (x,y): (" <<
		//rt.x << "," << rt.y << ")";

	//Update the control inputs for the vehicle.
	if (gMimicKeyInputs)
	{
		PxVehicleDrive4WSmoothDigitalRawInputsAndSetAnalogInputs(gKeySmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, timestep, gIsVehicleInAir, *gVehicle4W);
	}
	else
	{
		PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, timestep, gIsVehicleInAir, *gVehicle4W);
	}

	//Raycasts.
	PxVehicleWheels* vehicles[1] = { gVehicle4W };
	PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
	const PxU32 raycastResultsSize = gVehicleSceneQueryData->getQueryResultBufferSize();
	PxVehicleSuspensionRaycasts(gBatchQuery, 1, vehicles, raycastResultsSize, raycastResults);

	//Vehicle update.
	//const PxVec3 grav = GetScene(&px)->getGravity();
	const PxVec3 grav = PxVec3(0.0, -14, 0.0);
	PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
	PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()} };
	PxVehicleUpdates(timestep, grav, *gFrictionPairs, 1, vehicles, vehicleQueryResults);

	//Work out if the vehicle is in the air.
	gIsVehicleInAir = gVehicle4W->getRigidDynamicActor()->isSleeping() ? false : PxVehicleIsInAir(vehicleQueryResults[0]);
}

const PxVec3 VehiclePhysics::CurrentPosition()
{
	return gVehicle4W->getRigidDynamicActor()->getGlobalPose().p;
}

void VehiclePhysics::applyBoost() {

	if (boostTime == 0 && !useAI) {
		Sound::Play("sounds//boost.wav", 0.1f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);

	}

	PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
	DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[2];
	PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), 0, DirectX::XMVectorGetZ(mat));

	PxVec3 currentVel = gVehicle4W->getRigidDynamicActor()->getLinearVelocity();
	gVehicle4W->getRigidDynamicActor()->addForce(100000.f * forward);
	if (!gIsVehicleInAir) {
		//gVehicle4W->getRigidDynamicActor()->addForce(PxVec3(0, -50.0, 0));
	}
}

void VehiclePhysics::spinOut()
{
	if (spinOutTime == 0 && !useAI) {
		Sound::Play("sounds//pipe.wav", 0.6f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
	}

	PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
	DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[2];
	PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), 0, DirectX::XMVectorGetZ(mat));

	PxVec3 currentVel = gVehicle4W->getRigidDynamicActor()->getLinearVelocity();
	//gVehicle4W->getRigidDynamicActor()->addForce(-40000.f * forward);

	PxVec3 ang_vel = gVehicle4W->getRigidDynamicActor()->getAngularVelocity();
	gVehicle4W->getRigidDynamicActor()->setAngularVelocity(PxVec3(0.0f, 25.3, 0.0f));
	//gVehicle4W->getRigidDynamicActor()->addForce(PxVec3(0, -10000.0f, 0));

}

void VehiclePhysics::checkLaps(Entity* entity)
{
	PxVec3 pos = gVehicle4W->getRigidDynamicActor()->getGlobalPose().p;

	//Passed checkpoint 1
	if (!checkPoint1 && !checkPoint2 && !checkPoint3 && (pos.x >= -60 && pos.x <= -30) && (pos.y >= 15 && pos.y <= 30) && (pos.z >= 225 && pos.z <= 280)) {
		checkPoint1 = true;
	}

	//Passed checkpoint 2
	if (checkPoint1 && !checkPoint2 && !checkPoint3 && (pos.x >= 250 && pos.x <= 310) && (pos.y >= 15 && pos.y <= 30) && (pos.z >= -100 && pos.z <= -55)) {
		checkPoint2 = true;
	}

	//Passed checkpoint 3
	if (checkPoint1 && checkPoint2 && !checkPoint3 && (pos.x >= 60 && pos.x <= 110) && (pos.y >= 20 && pos.y <= 30) && (pos.z >= 125 && pos.z <= 180)) {
		checkPoint3 = true;
	}

	//Passed checkpoint 4
	if (checkPoint1 && checkPoint2 && checkPoint3 && (pos.x >= -160 && pos.x <= -110) && (pos.y >= 7 && pos.y <= 14) && (pos.z >= -125 && pos.z <= -70)) {
		checkPoint1 = false;
		checkPoint2 = false;
		checkPoint3 = false;
		entity->CountNumLaps();
		if (entity->GetNumLaps() == 3) {
			entity->haveWon++;
			entity->setFinishedIn(entity->haveWon);
		}
	}

	//std::stringstream ss;
	//ss << checkPoint1 << "   :   " << checkPoint2 << "  :  " << checkPoint3 << " Laps: " << entity->GetNumLaps() << " Has Won: " << entity->haveWon;
	//Gui::AddText(ss.str().c_str());

}

