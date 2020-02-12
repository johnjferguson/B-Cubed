#include "MissilePhysics.h"

using namespace physx;

MissilePhysics::MissilePhysics(PhysicsScene * ps, PxVec3& startPos)
	:
	ps(*ps)
{
	mis = createDynamic(PxTransform(startPos), PxSphereGeometry(10), PxVec3(0, -50, -100), ps);
	mis->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void MissilePhysics::Update(DirectX::XMFLOAT3 & pos, DirectX::XMMATRIX & transform)
{
	PxVec3 missile_position = mis->getGlobalPose().p;

	pos = DirectX::XMFLOAT3(missile_position.x, missile_position.y, missile_position.z);

	PxQuat quint = mis->getGlobalPose().q;
	transform = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w));
}

PxRigidDynamic* MissilePhysics::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity, PhysicsScene* ps)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*ps->gPhysics, t, geometry, *ps->gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	ps->gScene->addActor(*dynamic);
	return dynamic;
}