#include "MissilePhysics.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"

using namespace physx;

MissilePhysics::MissilePhysics(Physics* px, const PxTransform& transform, const PxVec3& velocity, const PxVec3& dimensions)
	:
	PhysicsDynamic(px, transform, velocity, dimensions)
{
	gRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void MissilePhysics::Update(Entity* entity)
{
	
	PxVec3 v = gRigidDynamic->getLinearVelocity().getNormalized();
	if (v.y > 0.f) {
		gRigidDynamic->setLinearVelocity(PxVec3(v.x, 0.0, v.z) * 75.f);
	}

	PhysicsDynamic::Update(entity);
}
