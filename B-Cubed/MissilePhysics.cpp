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
	PhysicsDynamic::Update(entity);
}
