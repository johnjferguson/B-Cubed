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
}

void PhysicsScene::Update(Time & dt)
{
	//Scene update.
	gScene->simulate(1.0f/60.0f);
	gScene->fetchResults(true);
}
