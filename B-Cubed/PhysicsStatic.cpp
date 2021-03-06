#include "PhysicsStatic.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"

using namespace physx;
using namespace snippetvehicle;

PhysicsStatic::PhysicsStatic(Physics* px, const PxTransform & transform, const PxVec3 & dimensions)
{
	gRigidStatic = PxCreateStatic(*GetPhysics(px), transform, PxBoxGeometry(dimensions), *GetMaterial(px));

	PxFilterData obstFilterData(COLLISION_FLAG_OBSTACLE, COLLISION_FLAG_OBSTACLE_AGAINST, 0, 0);
	PxShape* shapes[1];
	gRigidStatic->getShapes(shapes, 1);
	shapes[0]->setSimulationFilterData(obstFilterData);

	GetScene(px)->addActor(*gRigidStatic);
}

PhysicsStatic::~PhysicsStatic()
{
	gRigidStatic->release();
}

void PhysicsStatic::SetPosition(const DirectX::XMFLOAT3 & position)
{
	PxTransform current = gRigidStatic->getGlobalPose();
	current.p = PxVec3(position.x, position.y, position.z);
	gRigidStatic->setGlobalPose(current);
}

void PhysicsStatic::SetVelocity(const DirectX::XMFLOAT3 & velocity)
{
	assert(false && "setting velocity of static object");
}

void PhysicsStatic::Update(Entity* entity, const Time& dt)
{
	gRigidStatic->userData = (void*)entity;

	PxTransform transform = gRigidStatic->getGlobalPose();

	PxVec3 position = transform.p;
	entity->SetPosition(position.x, position.y, position.z);

	PxQuat quint = transform.q;
	entity->SetTransform(DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)));
}
