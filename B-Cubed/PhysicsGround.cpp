#include "PhysicsGround.h"
#include <assert.h>
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleSceneQuery.h"
#include "physx/vehicle4W/snippetvehiclecommon/SnippetVehicleFilterShader.h"

using namespace physx;
using namespace snippetvehicle;

PhysicsGround::PhysicsGround(Physics * px, const physx::PxTransform & transform, const std::vector<DirectX::XMFLOAT3>& vertices, const std::vector<unsigned short>& indices)
{
	gRigidStatic = GetPhysics(px)->createRigidStatic(transform);

	PxTriangleMeshDesc tmd;

	tmd.points.data = reinterpret_cast<const void*>(vertices.data());
	tmd.points.count = (PxU32)vertices.size();
	tmd.points.stride = (PxU32)sizeof(DirectX::XMFLOAT3);

	tmd.flags = PxMeshFlag::Enum::e16_BIT_INDICES;

	tmd.triangles.data = reinterpret_cast<const void*>(indices.data());
	tmd.triangles.count = (PxU32)indices.size() / 3;
	tmd.triangles.stride = 3 * sizeof(unsigned short);

	bool w = tmd.isValid();

	PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	//PxFilterData obstFilterData(COLLISION_FLAG_OBSTACLE, COLLISION_FLAG_OBSTACLE_AGAINST, 0, 0);
	PxFilterData qryFilterData;
	qryFilterData.word3 = static_cast<PxU32>(DRIVABLE_SURFACE);

	GetScene(px)->addActor(*gRigidStatic);


	PxDefaultMemoryOutputStream buf;
	PxTriangleMesh* pMesh;
	if (GetCooking(px)->cookTriangleMesh(tmd, buf))
	{
		PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
		pMesh = GetPhysics(px)->createTriangleMesh(id);

		PxShape* shape = PxRigidActorExt::createExclusiveShape(*gRigidStatic, PxTriangleMeshGeometry(pMesh), *GetMaterial(px));
		shape->setQueryFilterData(qryFilterData);
		shape->setSimulationFilterData(groundPlaneSimFilterData);
		//shape->setSimulationFilterData(obstFilterData);
		shape->setLocalPose(transform);

		GetScene(px)->addActor(*gRigidStatic);
	}
	else
	{
		assert(false && "cooking failed physics ground");
	}

}

void PhysicsGround::SetPosition(const DirectX::XMFLOAT3 & position)
{
	PhysicsStatic::SetPosition(position);
}

void PhysicsGround::SetVelocity(const DirectX::XMFLOAT3 & velocity)
{
	PhysicsStatic::SetVelocity(velocity);
}

void PhysicsGround::Update(Entity * entity, const Time& dt)
{
	gRigidStatic->userData = (void*)entity;

	PhysicsStatic::Update(entity, dt);
}
