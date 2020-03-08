#include "Physics.h"
#include <vector>
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"
#include "Entity.h"
#include "Sound.h"

#define PVD_HOST "127.0.0.1"
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

using namespace physx;
using namespace snippetvehicle;

std::vector<PxVec3> gContactPositions;
std::vector<PxVec3> gContactImpulses;

PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return PxFilterFlag::eDEFAULT;
}


class ContactReportCallback : public PxSimulationEventCallback
{
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		PX_UNUSED((pairHeader));
		std::vector<PxContactPairPoint> contactPoints;

		PxRigidActor* actor0 = pairHeader.actors[0];
		PxRigidActor* actor1 = pairHeader.actors[1];

		Entity* entity0 = (Entity*)actor0->userData;
		Entity* entity1 = (Entity*)actor1->userData;


		if (entity0->GetType() == Entity::Type::VEHICLE && entity1->GetType() == Entity::Type::MISSILE)
		{
			if (entity0->CanParry()) {
			//if (true) {
				entity1->SetBounceBack(true);
				Sound::Play("sounds//bulletbounce.wav", 0.5, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				entity1->ResetHit();
			}
			else if (entity0->CanBlock()) {
				entity1->MarkForDeath();
			}
			else {
				entity1->MarkForDeath();
				entity0->SetSpinOut(true);
			}
		}
		else if (entity0->GetType() == Entity::Type::MISSILE && entity1->GetType() == Entity::Type::VEHICLE)
		{
			if (entity1->CanParry()) {
			//if (true) {
				entity0->SetBounceBack(true);
				Sound::Play("sounds//bulletbounce.wav", 0.5, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				entity0->ResetHit();
			}
			else if (entity1->CanBlock()) {
				entity0->MarkForDeath();
			}
			else {
				entity0->MarkForDeath();
				entity1->SetSpinOut(true);
			}
			//entity0->MarkForDeath();
		}
		else if (entity0->GetType() == Entity::Type::MISSILE || entity1->GetType() == Entity::Type::MISSILE) 
		{
			if (entity0->GetType() == Entity::Type::MISSILE) {
				if (entity0->NumberofHits() > 3) {
					entity0->MarkForDeath();
				}
				else if (entity0->lastHitCounter > 5){
					Sound::Play("sounds//bulletbounce.wav", 0.5, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
					entity0->IncreaseHit();
					entity0->lastHitCounter = 0;
				}
			}
			else {
				if (entity1->NumberofHits() > 3) {
					entity1->MarkForDeath();
				}
				else if (entity1->lastHitCounter > 5){
					Sound::Play("sounds//bulletbounce.wav", 0.5, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
					entity1->IncreaseHit();
					entity1->lastHitCounter = 0;
				}
			}
		}


		for (PxU32 i = 0; i < nbPairs; i++)
		{
			PxU32 contactCount = pairs[i].contactCount;
			if (contactCount)
			{
				contactPoints.resize(contactCount);
				pairs[i].extractContacts(&contactPoints[0], contactCount);

				for (PxU32 j = 0; j < contactCount; j++)
				{
					gContactPositions.push_back(contactPoints[j].position);
					gContactImpulses.push_back(contactPoints[j].impulse);
				}
			}
		}
	}
};

ContactReportCallback gContactReportCallback;


Physics::Physics()
{
	//PxContactPairHeader anything;
	//anything.

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	PxU32 numWorkers = 1;
	gDispatcher = PxDefaultCpuDispatcherCreate(numWorkers);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = VehicleFilterShader;
	//sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;

	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(5.0f, 5.0f, -15.5f);
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));
}

Physics::~Physics()
{
	PX_RELEASE(gMaterial);
	PX_RELEASE(gCooking);
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
}

void Physics::Simulate(const Time & dt)
{
	// simulate the physics scene at a constant time
	current += dt();
	while (current > timestep)
	{
		gScene->simulate(timestep);
		current -= timestep;
	}
}

void Physics::Fetch()
{
	if (gContactPositions.size() > 0)
	{
		int i = 0;
	}
	gScene->fetchResults(true);
}

