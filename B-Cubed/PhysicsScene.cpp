#include "PhysicsScene.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"

using namespace physx;
using namespace snippetvehicle;

PhysicsScene::PhysicsScene()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	//gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	PxU32 numWorkers = 1;
	gDispatcher = PxDefaultCpuDispatcherCreate(numWorkers);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = VehicleFilterShader;

	gScene = gPhysics->createScene(sceneDesc);
	//static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	//---------------------------
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	//PxRigidStatic* wall1 = PxCreatePlane(* gPhysics, PxPlane(1, 0, 0, 0), * gMaterial);
	
	//Get the plane shape so we can set query and simulation filter data.
	//PxShape* shapes[1];
	//wall1->getShapes(shapes, 1);

	//Set the query filter data of the ground plane so that the vehicle raycasts can hit the ground.
	//PxFilterData qryFilterData;
	//setupDrivableSurface(qryFilterData);
	//shapes[0]->setQueryFilterData(qryFilterData);

	//Set the simulation filter data of the ground plane so that it collides with the chassis of a vehicle but not the wheels.
	//shapes[0]->setSimulationFilterData(simFilterData);

	//gScene->addActor(*wall1);
	//Create a plane to drive on.
	//PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	//PxRigidStatic* wall1 = PxCreatePlane(*gPhysics, PxPlane(10, 0, 10, 0), *gMaterial);
	//gScene->addActor(*wall1);

}

void PhysicsScene::Update(Time & dt)
{
	//Scene update.
	gScene->simulate(1.0f/60.0f);
	gScene->fetchResults(true);
}

