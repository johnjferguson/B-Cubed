#include "PhysicsDynamic.h"
#include "Entity.h"

using namespace physx;

PhysicsDynamic::PhysicsDynamic(Physics* phy, const physx::PxVec3& position, const physx::PxVec3& velocity, const physx::PxVec3& dimensions)
{
	PxTransform transform = PxTransform(position);
	gRigidDynamic = PxCreateDynamic(*phy->gPhysics, transform, PxBoxGeometry(dimensions), *phy->gMaterial, 10.0f);
	int i = 0;
	gRigidDynamic->setAngularDamping(0.5f);
	gRigidDynamic->setLinearVelocity(velocity);
	// should be able to set mass individually
	gRigidDynamic->setMass(1000.0f);
	phy->gScene->addActor(*gRigidDynamic);
}

PhysicsDynamic::~PhysicsDynamic()
{
	gRigidDynamic->release();
}

void PhysicsDynamic::Update(Entity * entity)
{
	PxVec3 pos = gRigidDynamic->getGlobalPose().p;
	PxQuat quat = gRigidDynamic->getGlobalPose().q;

	entity->SetPosition(pos.x, pos.y, pos.z);
	entity->SetTransform(DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quat.x, quat.y, quat.z, quat.w)));
}
