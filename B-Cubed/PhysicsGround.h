#pragma once
#include <DirectXMath.h>
#include "PhysicsStatic.h"
#include "Entity.h"


class PhysicsGround : public PhysicsStatic
{
public:
	PhysicsGround(Physics* px, const physx::PxTransform& transform, const std::vector<DirectX::XMFLOAT3>& vertices, const std::vector<unsigned short>& indices);
	virtual ~PhysicsGround() = default;
	virtual void SetPosition(const DirectX::XMFLOAT3& position) override;
	virtual void SetVelocity(const DirectX::XMFLOAT3& velocity) override;
	virtual void Update(Entity* entity) override;
private:
};