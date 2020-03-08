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
    PxVec3 vHorizontal = PxVec3(v.x, 0.0, v.y).getNormalized();

	if (v.y != 0.f) {
		gRigidDynamic->setLinearVelocity(PxVec3(v.x, 0.0, v.z) * 75.f);
		//gRigidDynamic->setLinearVelocity(vHorizontal * 75.f);
	}
	
	if (entity->GetBounceBack()) {

		PxVec3 bounceDir = entity->GetBounceDir();
		gRigidDynamic->setLinearVelocity(PxVec3(-bounceDir.x, 0.0, -bounceDir.z));
		PxVec3 currentPos = gRigidDynamic->getGlobalPose().p;
		gRigidDynamic->setGlobalPose(PxTransform(currentPos - bounceDir * 0.01));
		entity->SetBounceBack(false);
	}
	
	entity->lastHitCounter++;

	entity->SetBounceDir(gRigidDynamic->getLinearVelocity());

	PhysicsDynamic::Update(entity);

	/*
	gRigidDynamic->userData = (void*)entity;

	PxTransform transform = gRigidDynamic->getGlobalPose();

	PxVec3 position = transform.p;
	entity->SetPosition(position.x, position.y, position.z);

	PxQuat quint = transform.q;
	DirectX::XMMATRIX matrixTransform;
	DirectX::XMVECTOR ny = DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f);
	DirectX::XMVECTOR nz = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f);
	DirectX::XMVECTOR nx = DirectX::XMVector2Cross(nz, ny);
	DirectX::XMVECTOR nd = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);

	matrixTransform.r[0] = nx;
	matrixTransform.r[1] = ny;
	matrixTransform.r[2] = nz;
	matrixTransform.r[3] = nd;
	entity->SetTransform(DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)));
	
	entity->SetTransform(matrixTransform);
	*/
}


