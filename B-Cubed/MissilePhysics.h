#pragma once
#include "PhysicsComponent.h"
#include "physx/include/PxPhysicsAPI.h"
#include "Time.h"
#include "physx/vehicle4W/snippetcommon/SnippetPVD.h"
#include "physx/include/vehicle/PxVehicleUtil.h"
#include "physx/include/snippetutils/SnippetUtils.h"
#include <vector>
#include <DirectXMath.h>
#include "PhysicsScene.h"
#include <vector>
#include "Entity.h"
#include "Game.h"

class MissilePhysics : public PhysicsComponent
{
public:
	MissilePhysics(PhysicsScene* ps, physx::PxVec3& startPos, physx::PxQuat& startRot);
	virtual void Update(DirectX::XMFLOAT3& pos, DirectX::XMMATRIX& transform) override;
private:
	physx::PxRigidDynamic* createDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, const physx::PxVec3& velocity, PhysicsScene* ps);
private:
	physx::PxRigidDynamic* mis;
	PhysicsScene& ps;
};
