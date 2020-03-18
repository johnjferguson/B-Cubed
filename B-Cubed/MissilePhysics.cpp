#include "MissilePhysics.h"
#include "Physics.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"
#include <sstream>

using namespace physx;

MissilePhysics::MissilePhysics(Physics* px, const PxTransform& transform, const PxVec3& velocity, const PxVec3& dimensions)
	:
	PhysicsDynamic(px, transform, velocity, dimensions)
{
	phy = px;

	gRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//gRigidDynamic->setHitFlag(PxHitFlag::ePOSITION, true);
	//gRigidDynamic->setActorFlags.

}

void MissilePhysics::Update(Entity* entity)
{

	PxVec3 v = gRigidDynamic->getLinearVelocity().getNormalized();
    PxVec3 vHorizontal = PxVec3(v.x, 0.0, v.y).getNormalized();

	PxVec3 p = gRigidDynamic->getGlobalPose().p;
	PxQuat q = gRigidDynamic->getGlobalPose().q;

	PxRaycastBuffer hit;
	bool status = phy->gScene->raycast(PxVec3(p.x, p.y - 3, p.z), PxVec3(0, -1, 0), 50, hit);

	float dist = hit.block.distance;

	gRigidDynamic->setGlobalPose(PxTransform(p.x, p.y - (dist - 1), p.z, q));

	//std::stringstream ss;
	//ss << "Status: " << status << "Distance: " << hit.block.distance;
	//Gui::AddText(ss.str().c_str());
	
	if (v.y != 0.f) {
		gRigidDynamic->setLinearVelocity(PxVec3(v.x, 0.1, v.z) * 85.f);
	}
	
	if (entity->GetBounceBack()) {

		PxVec3 bounceDir = entity->GetBounceDir();
		gRigidDynamic->setLinearVelocity(PxVec3(-bounceDir.x, 0.0, -bounceDir.z));
		PxVec3 currentPos = gRigidDynamic->getGlobalPose().p;
		gRigidDynamic->setGlobalPose(PxTransform(currentPos - bounceDir * 0.01));
		entity->SetBounceBack(false);
	}
	
	entity->lastHitCounter++;

	entity->SetBounceDir(gRigidDynamic->getLinearVelocity());

	PhysicsDynamic::Update(entity);

}


