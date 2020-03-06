#include "VehiclePhysics.h"
#include "Gui.h"
#include <sstream>
#include "Time.h"

using namespace physx;
using namespace snippetvehicle;

VehiclePhysics::VehiclePhysics(Physics* px, Controller& gameController, Game* game, bool useAI, float startPosX, float startPosZ)
	:
	gameController(gameController),
	px(*px)
{
	VehiclePhysics::useAI = useAI;
	if (useAI) {
		std::vector<physx::PxVec3> p;
		p.push_back(physx::PxVec3(30.f, 0.f, 30.f));
		p.push_back(physx::PxVec3(-30.f, 0.f, 30.f));
		p.push_back(physx::PxVec3(-30.f, 0.f, -30.f));
		p.push_back(physx::PxVec3(30.f, 0.f, -30.f));
		VehiclePhysics::ai = AI::AI(p);
	}

	VehiclePhysics::game = game;
	VehiclePhysics::startPosX = startPosX;
	VehiclePhysics::startPosZ = startPosZ;
	
	gSteerVsForwardSpeedData =
	{
		0.0f,		0.75f,
		5.0f,		0.75f,
		30.0f,		0.2f,
		120.0f,		0.75f,
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

void VehiclePhysics::Update(Entity* entity)
{
	gVehicle4W->getRigidDynamicActor()->userData = (void*)entity;

	PxTransform transform = gVehicle4W->getRigidDynamicActor()->getGlobalPose();

	PxVec3 position = transform.p;
	entity->SetPosition(position.x, position.y, position.z);

	PxQuat quint = transform.q;
	entity->SetTransform(DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)));

	auto j = gVehicle4W->mDriveDynData.getCurrentGear();
	auto p = gVehicle4W->mDriveDynData.getEngineRotationSpeed();
	auto m = gVehicle4W->computeForwardSpeed();

	std::stringstream ss;
	ss << int(j) << "   :  " << (int)p << " Forwards Velocity:  " << (int)m;
	Gui::AddText(ss.str().c_str());



	stepPhysics(entity);
}

void VehiclePhysics::initVehicle(Physics* px)
{

	PxInitVehicleSDK(*GetPhysics(px));
	PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
	PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);

	//Create the batched scene queries for the suspension raycasts.
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, 1, WheelSceneQueryPreFilterBlocking, NULL, *GetAllocator(px));
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, GetScene(px));

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = createFrictionPairs(GetMaterial(px));

	//Create a plane to drive on.
    PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	gGroundPlane = createDrivablePlane(groundPlaneSimFilterData, GetMaterial(px), GetPhysics(px));
	GetScene(px)->addActor(*gGroundPlane);

	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initVehicleDesc(px);
	gVehicle4W = createVehicle4W(vehicleDesc, GetPhysics(px), GetCooking(px));
	PxTransform startTransform(PxVec3(startPosX, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius - 12.f), startPosZ), PxQuat(PxIdentity));
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	GetScene(px)->addActor(*gVehicle4W->getRigidDynamicActor());

	PxVehicleEngineData eng = gVehicle4W->mDriveSimData.getEngineData();
	eng.mPeakTorque = 800.f;
	eng.mMaxOmega = 500;
	//eng.mTorqueCurve = 1;
	//eng.mMOI = 5;

	PxVehicleAckermannGeometryData acker = gVehicle4W->mDriveSimData.getAckermannGeometryData();
	acker.mAccuracy = 1.0;

	// Higer Values = More Toque To Wheels
	PxVehicleClutchData clu = gVehicle4W->mDriveSimData.getClutchData();
	clu.mStrength = gVehicle4W->mDriveSimData.getClutchData().mStrength + 0;

	PxVehicleDifferential4WData diff = gVehicle4W->mDriveSimData.getDiffData();
	diff.mType = PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD;

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
	PxTransform com = gVehicle4W->getRigidDynamicActor()->getCMassLocalPose();
	PxTransform offset = PxTransform(PxVec3(0.0, -1.5, 0.0) + com.p);
	gVehicle4W->getRigidDynamicActor()->setCMassLocalPose(offset);

	gVehicleModeTimer = 0.0f;
	gVehicleOrderProgress = 0;
	//startBrakeMode();
}

snippetvehicle::VehicleDesc VehiclePhysics::initVehicleDesc(Physics* px)
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	//const PxF32 chassisMass = 1260.0f;
	const PxF32 chassisMass = 1800.0f;
	const PxVec3 chassisDims(5.0f, 3.0f, 5.5f);
	//const PxVec3 chassisDims(5.0f, 4.0f, 7.0f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.7f*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65, 0.2f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 100.0f;
	const PxF32 wheelRadius = 0.6f;
	const PxF32 wheelWidth = 1.0f;
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
	bool accel;
	bool reverse;
	float steer;

	if (useAI) {
		PxVec3 vel = gVehicle4W->getRigidDynamicActor()->getLinearVelocity();
		PxVec3 vehicle_position = gVehicle4W->getRigidDynamicActor()->getGlobalPose().p;
		PxQuat quint = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
		DirectX::XMVECTOR dirVec = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)).r[2];

		ai.update(vehicle_position, vel, dirVec);

		accel = ai.getAcceleration();
		reverse = ai.getBrake();
		steer = ai.getSteering();
	} else {
		accel = gameController.IsPressed(Controller::Button::R_TRIGGER);
		reverse = gameController.IsPressed(Controller::Button::L_TRIGGER);
		steer = gameController.GetLeftStick().x;

		blast = gameController.IsPressed(Controller::Button::Y);
		boost = gameController.IsPressed(Controller::Button::B);
		barrier = gameController.IsPressed(Controller::Button::A);
	}

	const PxF32 timestep = 1.0f / 60.0f;

	abilityTime++;
	entity->CountBarrier();

	//If full charge we don't start counting
	if (abilityCharges >= 3) {
		rechargeTime = 0;
	}
	else {
		rechargeTime++;
	}

	//Add an ability charge 
	if (rechargeTime > 120 && abilityCharges < 3) {
		abilityCharges++;
		rechargeTime = 0;
	}
	
	//Cycle through the driving modes to demonstrate how to accelerate/reverse/brake/turn etc.
	//incrementDrivingMode(timestep);
	if (barrier)
	{
		if (abilityTime > 60 && abilityCharges > 0 && aOnPress) {
			entity->ResetBarrier();
		}

		aOnPress = false;
	}
	else
	{
		aOnPress = true;
	}

	if (boost)
	{
		if (abilityTime > 60 && abilityCharges > 0 && bOnPress) {
			if (!boosting) {
				boosting = true;
				boostTimer = 60;
				abilityTime = 0;
				abilityCharges--;
			}
		}
		bOnPress = false;
	}
	else
	{
		bOnPress = true;
	}
	if (boosting) {
		Gui::AddText("Currently Boosting");
		if (boostTimer < 0) {
			boosting = false;
		}
		else {
			applyBoost();
			boostTimer--;
		}
	}

	if (blast)
	{
		if (abilityTime > 60 && abilityCharges > 0 && yOnPress) {
			PxQuat transform = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			//DirectX::XMMATRIX transform = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w));

			game->fireMissile(gVehicle4W->getRigidDynamicActor()->getGlobalPose().p, transform, gVehicle4W->getRigidDynamicActor()->getLinearVelocity());

			abilityTime = 0;
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

			gVehicle4W->getRigidDynamicActor()->addForce(10000.f * forward);
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
	const PxVec3 grav = PxVec3(0.0, -18, 0.0);
	PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
	PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()} };
	PxVehicleUpdates(timestep, grav, *gFrictionPairs, 1, vehicles, vehicleQueryResults);

	//Work out if the vehicle is in the air.
	gIsVehicleInAir = gVehicle4W->getRigidDynamicActor()->isSleeping() ? false : PxVehicleIsInAir(vehicleQueryResults[0]);
}

void VehiclePhysics::applyBoost() {

	PxQuat currentRot = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
	DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(currentRot.x, currentRot.y, currentRot.z, currentRot.w)).r[2];
	PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat), 0, DirectX::XMVectorGetZ(mat));

	PxVec3 currentVel = gVehicle4W->getRigidDynamicActor()->getLinearVelocity();
	gVehicle4W->getRigidDynamicActor()->addForce(40000.f * forward);
	if (!gIsVehicleInAir) {
		gVehicle4W->getRigidDynamicActor()->addForce(PxVec3(0, -50.0, 0));
	}
}


