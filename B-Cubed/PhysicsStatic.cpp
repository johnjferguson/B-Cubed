#include "PhysicsStatic.h"
#include <DirectXMath.h>
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"

using namespace physx;

PhysicsStatic::PhysicsStatic(PhysicsScene * phy, const physx::PxVec3 & position, const physx::PxVec3 & dimensions)
{
	


	PxTransform transform = PxTransform(position);
	gRigidStatic = PxCreateStatic(*phy->gPhysics, transform, PxBoxGeometry(dimensions), *phy->gMaterial);

	PxFilterData obstFilterData(snippetvehicle::COLLISION_FLAG_OBSTACLE, snippetvehicle::COLLISION_FLAG_OBSTACLE_AGAINST, 0, 0);
	PxShape* shapes[1];
	gRigidStatic->getShapes(shapes, 1);
	shapes[0]->setSimulationFilterData(obstFilterData);

	phy->gScene->addActor(*gRigidStatic);
}

void PhysicsStatic::Update(DirectX::XMFLOAT3 & pos_in, DirectX::XMMATRIX & transform)
{
	PxVec3 pos = gRigidStatic->getGlobalPose().p;
	PxQuat quat = gRigidStatic->getGlobalPose().q;

	pos_in = DirectX::XMFLOAT3(pos.x, pos.y, pos.z);
	transform = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quat.x, quat.y, quat.z, quat.w));
}
