#include "PhysicsDynamic.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"

using namespace physx;
using namespace snippetvehicle;

PhysicsDynamic::PhysicsDynamic(Physics* px, const physx::PxTransform& transform, const physx::PxVec3& velocity, const physx::PxVec3& dimensions)
{
	gRigidDynamic = PxCreateDynamic(*GetPhysics(px), transform, PxBoxGeometry(dimensions), *GetMaterial(px), 10.0f);

	// set initial velocity
	gRigidDynamic->setAngularDamping(0.5f);
	gRigidDynamic->setLinearVelocity(velocity);

	PxFilterData obstFilterData(COLLISION_FLAG_OBSTACLE, COLLISION_FLAG_OBSTACLE_AGAINST, 0, 0);
	PxShape* shapes[1];
	gRigidDynamic->getShapes(shapes, 1);
	shapes[0]->setSimulationFilterData(obstFilterData);

	gRigidDynamic->setMass(1);

	GetScene(px)->addActor(*gRigidDynamic);
}

void PhysicsDynamic::SetPosition(const DirectX::XMFLOAT3 & position)
{
	PxTransform current = gRigidDynamic->getGlobalPose();
	current.p = PxVec3(position.x, position.y, position.z);
	gRigidDynamic->setGlobalPose(current);
}

void PhysicsDynamic::SetVelocity(const DirectX::XMFLOAT3 & velocity)
{
	gRigidDynamic->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
}

PhysicsDynamic::~PhysicsDynamic()
{
	gRigidDynamic->release();
}

void PhysicsDynamic::Update(Entity * entity, const Time& dt)
{
	gRigidDynamic->userData = (void*)entity;

	PxTransform transform = gRigidDynamic->getGlobalPose();

	PxVec3 position = transform.p;
	entity->SetPosition(position.x, position.y, position.z);

	PxQuat quint = transform.q;
	entity->SetTransform(DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)));

	// this is bad practice
	entity->SetVelocity(*(DirectX::XMFLOAT3*)&gRigidDynamic->getLinearVelocity());
}
