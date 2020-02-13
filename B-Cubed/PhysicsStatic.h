#pragma once
#include "PhysicsComponent.h"
#include "PhysicsScene.h"

class PhysicsStatic : public PhysicsComponent
{
public:
	PhysicsStatic(PhysicsScene* phy, const physx::PxVec3& position, const physx::PxVec3& dimensions);
	virtual void Update(DirectX::XMFLOAT3& pos, DirectX::XMMATRIX& transform) override;
private:
	physx::PxRigidStatic* gRigidStatic;
};