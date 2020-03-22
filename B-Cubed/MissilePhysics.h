#pragma once
#include "PhysicsDynamic.h"
#include "Entity.h"

class MissilePhysics : public PhysicsDynamic
{
public:
	MissilePhysics(Physics* px, const physx::PxTransform& transform, const physx::PxVec3& velocity, const physx::PxVec3& dimensions);
	virtual ~MissilePhysics() = default;
	virtual void SetPosition(const DirectX::XMFLOAT3& position) override;
	virtual void SetVelocity(const DirectX::XMFLOAT3& velocity) override;
	virtual void Update(Entity* entity, const Time& dt) override;
private:
	Physics* phy;
};
