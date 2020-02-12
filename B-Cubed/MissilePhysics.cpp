#include "MissilePhysics.h"
#include <DirectXMath.h>

using namespace physx;

MissilePhysics::MissilePhysics(PhysicsScene * ps, PxVec3& startPos, PxQuat& startRot, PxVec3& startVel)
	:
	ps(*ps)
{
	
	DirectX::XMVECTOR mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(startRot.x, startRot.y, startRot.z, startRot.w)).r[2];
	PxVec3 forward = PxVec3(DirectX::XMVectorGetX(mat) , 0, DirectX::XMVectorGetZ(mat) );

	mis = createDynamic(PxTransform(startPos + forward * 2.0f) , PxSphereGeometry(10), forward * 20.0f + startVel, ps);
	mis->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	PxShape* shape = ps->gPhysics->createShape(PxSphereGeometry(1.0f), *ps->gMaterial, true);
	mis->attachShape(*shape);
	//shape->release();

	//PxShape* shape = ps->gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	//mis->attachShape(*shape);
	//PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	//gScene->addActor(*body);
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