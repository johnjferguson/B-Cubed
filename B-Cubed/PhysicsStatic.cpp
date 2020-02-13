#include "PhysicsStatic.h"
#include <DirectXMath.h>
#include "Entity.h"

using namespace physx;

PhysicsStatic::PhysicsStatic(Physics * phy, const PxVec3 & position, const PxVec3 & dimensions)
{
	PxTransform transform = PxTransform(position);
	gRigidStatic = PxCreateStatic(*phy->gPhysics, transform, PxBoxGeometry(dimensions), *phy->gMaterial);
	phy->gScene->addActor(*gRigidStatic);
}

PhysicsStatic::~PhysicsStatic()
{
	gRigidStatic->release();
}

void PhysicsStatic::Update(Entity * entity)
{
	PxVec3 pos = gRigidStatic->getGlobalPose().p;
	PxQuat quat = gRigidStatic->getGlobalPose().q;

	entity->SetPosition(pos.x, pos.y, pos.z);
	entity->SetTransform(DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quat.x, quat.y, quat.z, quat.w)));
}
