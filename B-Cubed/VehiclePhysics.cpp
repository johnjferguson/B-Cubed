#include "VehiclePhysics.h"
#include "Gui.h"
#include <sstream>
#include "Time.h"

using namespace physx;
using namespace snippetvehicle;

VehiclePhysics::VehiclePhysics(PhysicsScene* ps, Controller& gameController, Game* game)
	:
	gameController(gameController),
	ps(*ps)
{
	VehiclePhysics::game = game;

	gSteerVsForwardSpeedData =
	{
		0.0f,		0.75f,
		5.0f,		0.75f,
		30.0f,		0.125f,
		120.0f,		0.1f,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32
	};

	gSteerVsForwardSpeedTable = PxFixedSizeLookupTable<8>(gSteerVsForwardSpeedData.data(), 4);
	gKeySmoothingData =
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

	initVehicle(ps);
}

void VehiclePhysics::Update(DirectX::XMFLOAT3 & pos, DirectX::XMMATRIX & transform)
{
	PxVec3 vel = gVehicle4W->getRigidDynamicActor()->getLinearVelocity();
	PxVec3 vehicle_position = gVehicle4W->getRigidDynamicActor()->getGlobalPose().p;

	pos = DirectX::XMFLOAT3(vehicle_position.x, vehicle_position.y, vehicle_position.z);
	PxQuat quint = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
	transform = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w));

	std::stringstream ss;
	ss << "Velocity of ball (x,y,z) = (" << (abs(vel.x) < 0.1 ? 0.0f : vel.x) << "," << (abs(vel.y) < 0.1 ? 0.0f : vel.y) << "," << (abs(vel.z) < 0.1 ? 0.0f : vel.z) << ")";
	Gui::AddText(ss.str());
	std::stringstream bb;
	bb << "Position of vehicle (x,y,z) = (" << vehicle_position.x << "," << vehicle_position.y << "," << vehicle_position.z << ")";
	Gui::AddText(bb.str());
	stepPhysics();
}

void VehiclePhysics::initVehicle(PhysicsScene* ps)
{
	
	/////////////////////////////////////////////

	PxInitVehicleSDK(*ps->gPhysics);
	PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
	PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);

	//Create the batched scene queries for the suspension raycasts.
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, 1, WheelSceneQueryPreFilterBlocking, NULL, ps->gAllocator);
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, ps->gScene);

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = createFrictionPairs(ps->gMaterial);

	//Create a plane to drive on.
	PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	gGroundPlane = createDrivablePlane(groundPlaneSimFilterData, ps->gMaterial, ps->gPhysics);
	ps->gScene->addActor(*gGroundPlane);

	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initVehicleDesc(ps);
	gVehicle4W = createVehicle4W(vehicleDesc, ps->gPhysics, ps->gCooking);
	PxTransform startTransform(PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), PxQuat(PxIdentity));
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	ps->gScene->addActor(*gVehicle4W->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	gVehicle4W->setToRestState();
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	gVehicle4W->mDriveDynData.setUseAutoGears(true);

	gVehicleModeTimer = 0.0f;
	gVehicleOrderProgress = 0;
	//startBrakeMode();
}

snippetvehicle::VehicleDesc VehiclePhysics::initVehicleDesc(PhysicsScene* ps)
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const PxF32 chassisMass = 1500.0f;
	const PxVec3 chassisDims(2.5f, 2.0f, 5.0f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 20.0f;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.4f;
	const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 6;

	VehicleDesc vehicleDesc;

	vehicleDesc.chassisMass = chassisMass;
	vehicleDesc.chassisDims = chassisDims;
	vehicleDesc.chassisMOI = chassisMOI;
	vehicleDesc.chassisCMOffset = chassisCMOffset;
	vehicleDesc.chassisMaterial = ps->gMaterial;
	vehicleDesc.chassisSimFilterData = PxFilterData(COLLISION_FLAG_CHASSIS, COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);

	vehicleDesc.wheelMass = wheelMass;
	vehicleDesc.wheelRadius = wheelRadius;
	vehicleDesc.wheelWidth = wheelWidth;
	vehicleDesc.wheelMOI = wheelMOI;
	vehicleDesc.numWheels = nbWheels;
	vehicleDesc.wheelMaterial = ps->gMaterial;
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

void VehiclePhysics::stepPhysics()
{
	const PxF32 timestep = 1.0f / 60.0f;
	readyToFire++;

	//Cycle through the driving modes to demonstrate how to accelerate/reverse/brake/turn etc.
	//incrementDrivingMode(timestep);
	if (gameController.IsPressed(Controller::Button::A))
	{
		Gui::AddText("A is pressed");
	}
	else
	{
		Gui::AddText("A is not pressed");
	}
	if (gameController.IsPressed(Controller::Button::B))
	{
		Gui::AddText("B is pressed");
	}
	else
	{
		Gui::AddText("B is not pressed");
	}
	if (gameController.IsPressed(Controller::Button::Y))
	{
		Gui::AddText("Y is pressed");
		if (readyToFire > 60 && yOnPress) {
			PxQuat transform = gVehicle4W->getRigidDynamicActor()->getGlobalPose().q;
			//DirectX::XMMATRIX transform = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w));

			game->fireMissile(gVehicle4W->getRigidDynamicActor()->getGlobalPose().p, transform);
			readyToFire = 0;


		}
		yOnPress = false;
	}
	else
	{
		Gui::AddText("Y is not pressed");
		yOnPress = true;
	}
	if (gameController.IsPressed(Controller::Button::X))
	{
		Gui::AddText("X is pressed");
	}
	else
	{
		Gui::AddText("X is not pressed");
	}
	if (gameController.IsPressed(Controller::Button::L_TRIGGER))
	{
		//releaseAllControls();
		gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);
		gVehicleInputData.setAnalogAccel(true);
	}
	else if (gameController.IsPressed(Controller::Button::R_TRIGGER))
	{
		gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
		gVehicleInputData.setAnalogAccel(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(false);
	}

	gVehicleInputData.setAnalogSteer(gameController.GetLeftStick().x);

	// TEMP: just testing stuff since I don't have xbox controller
	//gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
	//gVehicleInputData.setAnalogAccel(true);
	//gVehicleInputData.setAnalogSteer(0.5f);
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
	const PxVec3 grav = ps.gScene->getGravity();
	PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
	PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()} };
	PxVehicleUpdates(timestep, grav, *gFrictionPairs, 1, vehicles, vehicleQueryResults);

	//Work out if the vehicle is in the air.
	gIsVehicleInAir = gVehicle4W->getRigidDynamicActor()->isSleeping() ? false : PxVehicleIsInAir(vehicleQueryResults[0]);

	//ps.gScene->simulate(timestep);
	//ps.gScene->fetchResults(true);
	//Time t(1.0f / 60.0f);
	//ps.Update(t);
}
/*
//-----------------------------------------ADDED-----------------------------------------------------
void Physics::fireMissile(PxVec3 startPos)
{
	PxRigidDynamic* dyn = createDynamic(PxTransform(startPos), PxSphereGeometry(10), PxVec3(0, -50, -100));

	dyn->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);


	std::unique_ptr<VehiclePhysics> vp0 = std::make_unique<VehiclePhysics>(&ps, wnd.clr);
	std::unique_ptr<VehiclePhysics> vp1 = std::make_unique<VehiclePhysics>(&ps, wnd.clr);

	Game::entities[5].AddRenderable(std::move(nb));
	Game::entities[5].SetPosition(5.0f, 1.0f, 0.0f);
	Game::entities[5].AddPhysics(std::move(vp0));
}

//----------------------------------ADDED----------------------------------------
PxRigidDynamic* Physics::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}
*/
